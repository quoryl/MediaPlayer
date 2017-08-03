//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLER_H
#define DAEUM_MEDIACONTROLLER_H


#include "ControllerInterface.h"
#include "../Model/Song.h"

class MediaController: public ControllerInterface{

public:
    explicit MediaController( Song* s);
    void Search() override ;
    void Add() override ;
    void Delete() override ;
    void Shuffle() override ;
    void Previous() override ;
    void Play() override ;
    void Next() override ;
    void Repeat() override ;
    void showVolume() override ;
    void changeVolume() override ;
    void executeMenuItem() override ;
private:
    Song* song;
};


#endif //DAEUM_MEDIACONTROLLER_H
