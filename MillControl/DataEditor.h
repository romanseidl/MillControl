#ifndef MILLCONTROL_DATAEDITOR_H
#define MILLCONTROL_DATAEDITOR_H

#include "State.h"
#include "DataEditorData.h"

class DataEditor : public State {
protected:
    int *time;
    DataEditorData *editor;
public:
    DataEditor(DataEditorData *editor) : editor(editor) { }

    virtual bool start() override;

    virtual void encoderClick() override;

    virtual void draw() override;

    virtual void encoderChanged(int encoderPos) override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "DataEditor"; }
#endif
};

#endif //MILLCONTROL_DATAEDITOR_H