#pragma once

#include "ofxiOSExtras.h"
#include "ofxImGui.h"

class EditableText;

@interface TextField : UITextField<UIKeyInput>
{
@public
    EditableText* editableText;
}
- (void)textFieldEditingChanged:(id)sender;

@end







class EditableText
{
    
public:
    EditableText();
    bool hasText;
    void setup(string initialString);
    void begin();
    void end();
    char* getText();
    void onTextEdit(string);
    void onInsertText(string);
    void onTextDelete();
    TextField* textField;
    string inputTextValue;
    string currentText;
    char* internalBuffer;
    //EditableTextResponder* textResponder;
};
