#ifndef RG3_HITTABLE_LIST_H
#define RG3_HITTABLE_LIST_H

#include "hittable.h"

class hittable_list: public hittable{
public:
    hittable **list;
    int list_size;

    hittable_list() = default;
    hittable_list(hittable **l, int n): list(l), list_size(n) {}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &record) const;
};

bool hittable_list::hit(const class ray & r, double t_min, double t_max, hit_record& record) const {
    bool hit = false;
    double closest_so_far = t_max;
    hit_record temp_record;

    for(int i = 0; i < list_size; i++){
        if(list[i]->hit(r, t_min, closest_so_far, temp_record)){
            closest_so_far = temp_record.t;
            record = temp_record;
            hit = true;
        }
    }

    return hit;
}

#endif //RG3_HITTABLE_LIST_H
