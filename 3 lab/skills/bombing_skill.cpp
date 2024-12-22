#include <ctime>
#include <vector>
#include <utility>
#include <random>

#include "bombing_skill.h"


bool BombingSkill::use(InfoHolder& info_holder){
    random_device rd;
    default_random_engine engine(rd());

    vector<pair<size_t, size_t>> idxs;

    for (size_t i = 0; i < info_holder.playing_field->getHeight(); ++i)
        for (size_t j = 0; j < info_holder.playing_field->getWidth(); ++j)
            if (!info_holder.playing_field->getCell(j, i).isEmpty() &&
                info_holder.playing_field->getCell(j, i).getSegment().getState() != Segment::State::DESTROYED)
                idxs.push_back(pair<size_t, size_t>(i, j));
    
    if (idxs.empty())
        return false;
    size_t idx = engine() % idxs.size();

    bool hidden_flag = info_holder.playing_field->getCell(idxs[idx].second, idxs[idx].first).getState() == Cell::ViewState::HIDDEN;
    info_holder.playing_field->attack(idxs[idx].second, idxs[idx].first);
    if (hidden_flag){
        info_holder.playing_field->getCell(idxs[idx].second, idxs[idx].first).setState(Cell::ViewState::HIDDEN);
    }

    return true;
}
