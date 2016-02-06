#ifndef MILLCONTROL_SCALETAREPROMPT_H
#define MILLCONTROL_SCALETAREPROMPT_H

#include "Prompt.h"

class ScaleTarePrompt : public Prompt {
    long updateTime;
    long weight;

public:
    virtual bool open() override;

    virtual void loop() override;

    virtual void draw() override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "ScaleTarePrompt"; }
#endif

    virtual void ok() override;

    virtual bool start() override;

protected:
    virtual bool close() override;
};

#endif