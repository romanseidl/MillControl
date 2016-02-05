#ifndef MILLCONTROL_EDITOR_H
#define MILLCONTROL_EDITOR_H

#include "State.h"
#include "DataEditorData.h"

class MultiModeSelector;

class Editor : public State, public DataEditorData {
    int position = 0;

    unsigned char back = 0;
    unsigned char gram = 0;
    unsigned char add = 0;
    unsigned char del = 0;
    unsigned char left = 0;
    unsigned char right = 0;
    unsigned char firstChar = 0;

    bool deleteMode = false;

    Mode *timeMode;
    ModeList *timeModes;
public:
    virtual bool start() override;
    virtual void stop() override;

    void encoderClick();
    virtual void millClick(unsigned char i) override;
    virtual void encoderChanged(int encoderPos) override;

    virtual void draw();

    char *getChar();

    virtual int *getData() override;

    virtual void drawEditor() override;

private:

    void drawEditPoint(const unsigned char p, const unsigned char pos, const unsigned char symbol) const;

    void draw(bool editor);

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "Editor"; }
#endif
};


#endif //MILLCONTROL_EDITOR_H