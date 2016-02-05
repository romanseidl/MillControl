#ifndef MILLCONTROL_CALIBRATIONPROMPT_H
#define MILLCONTROL_CALIBRATIONPROMPT_H

#include "Prompt.h"
#include "DataEditorData.h"

class CalibrationPrompt : public Prompt, public DataEditorData {
protected:
    void draw(bool editor);

public:
    CalibrationPrompt();

    virtual bool open() override;

    virtual int *getData() override;

    int weight = 70;
private:
    virtual void drawEditor() override;

    virtual void encoderClick() override;

    virtual void draw() override;

protected:


public:
    virtual void cancel() override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "CalibrationPrompt"; }
#endif
};

#endif //MILLCONTROL_CALIBRATIONPROMPT_H