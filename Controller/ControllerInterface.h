//
// Created by azrael on 31/07/17.
//

#ifndef DAEUM_MEDIACONTROLLERINTERFACE_H
#define DAEUM_MEDIACONTROLLERINTERFACE_H


class ControllerInterface {
public:
    virtual void Search() = 0;
    virtual void Add() = 0;
    virtual void Delete() = 0;
    virtual void Shuffle() = 0;
    virtual void Previous() = 0;
    virtual void Play() = 0;
    virtual void Next() = 0;
    virtual void Repeat() = 0;
    virtual void showVolume() = 0;
    virtual void changeVolume() = 0;
    virtual void executeMenuItem() = 0;


};


#endif //DAEUM_MEDIACONTROLLERINTERFACE_H
