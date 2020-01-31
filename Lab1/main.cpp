#include <iostream>
#include <glut.h>
#include <vector>
#include <math.h>
#include <algorithm>

class Point{
public:
    double x_, y_, z_, h_;
    double Nx_, Ny_, Nz_;
    Point(double x, double y, double z, double h) : x_(x), y_(y), z_(z), h_(h){}

};

class MyPolygon{
public:
    int p1_, p2_, p3_;
    double A_, B_, C_, D_;
    double Nx_, Ny_, Nz_;
    MyPolygon(int p1, int p2, int p3) : p1_(p1), p2_(p2), p3_(p3){}

    void setNormal(Point p1, Point p2, Point p3){
        double v12[] = {p2.x_ - p1.x_, p2.y_ - p1.y_, p2.z_ - p1.z_};
        double v13[] = {p3.x_ - p1.x_, p3.y_ - p1.y_, p3.z_ - p1.z_};
        Nx_ = v12[1]*v13[2] - v12[2]*v13[1];
        Ny_ = -v12[0]*v13[2] + v12[2]*v13[0];
        Nz_ = v12[0]*v13[1] - v12[1]*v13[0];
        double norm = sqrt(Nx_ * Nx_ + Ny_ * Ny_ + Nz_ * Nz_);
        Nx_ /= norm; Ny_ /= norm; Nz_ /= norm;
    }
};

class Tangent{
public:
    Point p1_, p2_;
    Tangent(Point p1, Point p2) : p1_(p1), p2_(p2) {}
};

std::vector<Point> vertices;
std::vector<MyPolygon> polygons;
std::vector<Point> controlPoints;
std::vector<Point> curvePoints;
std::vector<Tangent> tangentToDraw;
std::vector<Point> tangents;
int numberOfSegments;
Point rotationAxis(0, 0, 0, 1);
double rotationAngle;
double xmin, xmax, ymin, ymax, zmin, zmax;
int currentPoint = 0;
Point objectCenter(0,0,0,1);
bool showTangents = false;
bool showControlPolygon = false;

GLuint width = 800;
GLuint height = 800;

Point O(40, 40, -15, 1.0);
Point G(0.0, 0.0, 0.0, 1.0);

void display();
void reshape(int width, int height);
void keyboard(unsigned char theKey, int mouseX, int mouseY);

void normalize(){
    xmin = xmax = vertices.at(0).x_;
    ymin = ymax = vertices.at(0).y_;
    zmin = zmax = vertices.at(0).z_;

    for(auto vertex : vertices){
        if(vertex.x_ < xmin) xmin = vertex.x_;
        if(vertex.x_ > xmax) xmax = vertex.x_;
        if(vertex.y_ < ymin) ymin = vertex.y_;
        if(vertex.y_ > ymax) ymax = vertex.y_;
        if(vertex.z_ < zmin) zmin = vertex.z_;
        if(vertex.z_ > zmax) zmax = vertex.z_;
    }

    double xs = (xmin + xmax) / 2., ys = (ymin + ymax) / 2., zs = (zmin + zmax) / 2.;
    objectCenter.x_= xs; objectCenter.y_ = ys; objectCenter.z_ = zs;

    double maximum = std::max(xmax - xmin, ymax - ymin);
    maximum = std::max(maximum, zmax - zmin);

    for (auto &point : vertices){
        point.x_ -= xs;
        point.x_ *= (5. / maximum);

        point.y_ -= ys;
        point.y_ *= (5. / maximum);

        point.z_ -= zs;
        point.z_ *= (5. / maximum);
    }

}

void calculateCurvePoints(){
    // Izračun točaka krivulje
    for(int i = 0; i < numberOfSegments; i++){
        Point r1 = controlPoints.at(i);
        Point r2 = controlPoints.at(i + 1);
        Point r3 = controlPoints.at(i + 2);
        Point r4 = controlPoints.at(i + 3);

        for(int j = 0; j < 100; j++){
            double t = j / 100.;

            double b1 = (-pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) / 6.0;
            double b2 = (3 * pow(t,3) - 6 * pow(t, 2) + 4) / 6.0;
            double b3 = (-3 * pow(t,3) + 3 * pow(t, 2) + 3 * t + 1) / 6.0;
            double b4 = (pow(t, 3)) / 6.0;

            double x = b1 * r1.x_ + b2 * r2.x_ + b3 * r3.x_ + b4 * r4.x_;
            double y = b1 * r1.y_ + b2 * r2.y_ + b3 * r3.y_ + b4 * r4.y_;
            double z = b1 * r1.z_ + b2 * r2.z_ + b3 * r3.z_ + b4 * r4.z_;

            Point p(x, y, z, 1);
            curvePoints.push_back(p);

            double d1 = (-pow(t, 2) + 2 * t - 1) / 2.0;
            double d2 = (3 * pow(t, 2) - 4 * t) / 2.0;
            double d3 = (-3 * pow(t,2) + 2 * t + 1) / 2.0;
            double d4 = pow(t, 2) / 2.0;

            double xd = d1 * r1.x_ + d2 * r2.x_ + d3 * r3.x_+ d4 * r4.x_;
            double yd = d1 * r1.y_ + d2 * r2.y_ + d3 * r3.y_+ d4 * r4.y_;
            double zd = d1 * r1.z_ + d2 * r2.z_ + d3 * r3.z_+ d4 * r4.z_;

            if(j == 0 || (j == 99)){
                Point p1(p.x_ - xd / 4, p.y_ - yd / 4, p.z_ - zd / 4, 1);
                Point p2(p.x_ + xd / 4, p.y_ + yd / 4, p.z_ + zd / 4, 1);
                tangentToDraw.push_back(Tangent(p1, p2));
            }
            tangents.push_back(Point(xd, yd, zd, 1));

        }
    }
}

void calculateRotation(){
    Point s(0, 0, 1, 1);
    Point c = tangents.at(currentPoint);

    double osx = s.y_ * c.z_ - c.y_ * s.z_;
    double osy = -s.x_ * c.z_ + c.x_ * s.z_;
    double osz = s.x_ * c.y_ - s.y_ * c.x_;
    rotationAxis.x_ = osx;
    rotationAxis.y_ = osy;
    rotationAxis.z_ = osz;
    rotationAxis.h_ = 1;

    double sc = s.x_ * c.x_ + s.y_ * c.y_ + s.z_ * c.z_;
    double sNorm = sqrt(s.x_ * s.x_ + s.y_ * s.y_ + s.z_ * s.z_);
    double cNorm = sqrt(c.x_ * c.x_ + c.y_ * c.y_ + c.z_ * c.z_);

    rotationAngle = acos(sc / (sNorm * cNorm)) * 180 / M_PI;
}

void moveBackward(){
    currentPoint += curvePoints.size() - 1;
    currentPoint %= curvePoints.size();
    calculateRotation();
}

void moveForward(){
    currentPoint++;
    currentPoint %= curvePoints.size();
    calculateRotation();
}

void calculateNormals(){
    for(MyPolygon &t : polygons){
        Point p1 = vertices.at(t.p1_ - 1);
        Point p2 = vertices.at(t.p2_ - 1);
        Point p3 = vertices.at(t.p3_ - 1);

        t.setNormal(p1, p2, p3);
    }
}

void initialize(){
    calculateCurvePoints();
    normalize();
    calculateNormals();
    moveBackward();
    moveForward();
}

int main(int argc, char** argv) {
    // Učitavanje objekta

    FILE *file = fopen(argv[1], "r");

    if (file == nullptr){
        std::cout << "Impossible to open the file 1!\n";
        return -1;
    }
    while (true){
        char line[128];
        // read the first word of the line
        int res = fscanf(file, "%s", line);
        if(res == EOF) break;
        if(strcmp(line, "v") == 0){
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            Point vertex(x, y, z, 1);

            vertices.push_back(vertex);
        } else if (strcmp(line, "f") == 0){
            int p1, p2, p3;
            fscanf(file, "%d%d%d\n", &p1, &p2, &p3);
            MyPolygon polygon(p1, p2, p3);
            polygons.push_back(polygon);
        }
    }
    fclose(file);
    for(auto& polygon : polygons){
        polygon.A_ = (vertices.at(polygon.p2_-1).y_ - vertices.at(polygon.p1_-1).y_) *
                     (vertices.at(polygon.p3_-1).z_ - vertices.at(polygon.p1_-1).z_) -

                     (vertices.at(polygon.p2_-1).z_ - vertices.at(polygon.p1_-1).z_)*
                     (vertices.at(polygon.p3_-1).y_ - vertices.at(polygon.p1_-1).y_);

        polygon.B_ = -1 * (vertices.at(polygon.p2_-1).x_ - vertices.at(polygon.p1_-1).x_) *
                     (vertices.at(polygon.p3_-1).z_ - vertices.at(polygon.p1_-1).z_) +

                     (vertices.at(polygon.p2_-1).z_ - vertices.at(polygon.p1_-1).z_)*
                     (vertices.at(polygon.p3_-1).x_ - vertices.at(polygon.p1_-1).x_);

        polygon.C_ = (vertices.at(polygon.p2_-1).x_ - vertices.at(polygon.p1_-1).x_) *
                     (vertices.at(polygon.p3_-1).y_ - vertices.at(polygon.p1_-1).y_) -

                     (vertices.at(polygon.p2_-1).y_ - vertices.at(polygon.p1_-1).y_)*
                     (vertices.at(polygon.p3_-1).x_ - vertices.at(polygon.p1_-1).x_);

        polygon.D_ = -vertices.at(polygon.p1_-1).x_ * polygon.A_
                     -vertices.at(polygon.p1_-1).y_ * polygon.B_
                     -vertices.at(polygon.p1_-1).z_ * polygon.C_;
    }

    // Učitavanje kontrolnih točaka krivulje
    FILE *file2 = fopen(argv[2], "r");
    if (file2 == nullptr){
        std::cout << "Impossible to open the file 2!\n";
        return -2;
    }
    while (true){
        char line[128];
        // read the first word of the line
        int res = fscanf(file, "%s", line);
        if(res == EOF) break;
        if(strcmp(line, "") != 0){
            float x, y, z;
            fscanf(file2, "%f%f%f", &x, &y, &z);
            Point controlPoint(x, y, z, 1);
            controlPoints.push_back(controlPoint);
        }
    }
    fclose(file2);
    numberOfSegments = controlPoints.size() - 3;
    initialize();

    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (50, 50);
    glutInit(&argc, argv);

    glutCreateWindow ("Racunalna grafika - lab1");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

void display(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glLoadIdentity();
    glPointSize(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Krivulja
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for(Point &t : curvePoints){
        glVertex3d(t.x_, t.y_, t.z_);
    }
    glEnd();

    //Tangente
    if(showTangents){
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2);
        glBegin(GL_LINES);
        for(Tangent &t : tangentToDraw){
            glVertex3d(t.p1_.x_, t.p1_.y_, t.p1_.z_);
            glVertex3d(t.p2_.x_, t.p2_.y_, t.p2_.z_);
        }
        glEnd();
        glLineWidth(1);
    }

    // Kontrolni poligon
    if(showControlPolygon){
        glColor3f(0.0f, 1.0f, 0.5f);
        glBegin(GL_LINE_STRIP);
        for(Point &t : controlPoints){
            glVertex3d(t.x_, t.y_, t.z_);
        }
        glEnd();
    }

    // Objekt
    glColor3f(0.0f, 0.4f, 0.4f);
    Point current = curvePoints.at(currentPoint);
    glTranslated(current.x_, current.y_, current.z_);
    glRotated(rotationAngle, rotationAxis.x_, rotationAxis.y_, rotationAxis.z_);
    for(MyPolygon &t : polygons){
        glBegin(GL_LINE_STRIP);
        Point vertex1 = vertices.at(t.p1_ - 1);
        glVertex3d(vertex1.x_, vertex1.y_, vertex1.z_);

        Point vertex2 = vertices.at(t.p2_ - 1);
        glVertex3d(vertex2.x_, vertex2.y_, vertex2.z_);

        Point vertex3 = vertices.at(t.p3_ - 1);
        glVertex3d(vertex3.x_, vertex3.y_, vertex3.z_);
        glEnd();
    }
    glutSwapBuffers();
}

void reshape(int w, int h){
    width = h; height = h;
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float) width / height;
    gluPerspective(45.0f, aspect, 0.5f, 150.0f);
    gluLookAt(O.x_, O.y_, O.z_, 0, 0, 30, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char theKey, int mouseX, int mouseY){
    switch(theKey){
        case 'w':
            moveForward();
            break;
        case 's':
            moveBackward();
            break;
        case 't':
            showTangents = !showTangents;
            break;
        case 'k':
            showControlPolygon = !showControlPolygon;
            break;
    }
    glutPostRedisplay();
}