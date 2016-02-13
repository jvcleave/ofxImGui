#pragma once

#include "ofxiOSExtras.h"
#include "ofxImGui.h"

class InputTextButton;

@interface TextFieldDelegate : NSObject<UITextFieldDelegate>
{
@public
    InputTextButton* inputTextButton;
}
- (void)textFieldEditingChanged:(id)sender;
- (BOOL)textFieldShouldReturn:(UITextField *)textField;

@end

class InputTextButton
{
    
public:
    InputTextButton();
    void setup(string initialString);
    void draw();
    void onTextEdit(string);
    UITextField* textField;
    TextFieldDelegate* textFieldDelegate;
    string text;
};
