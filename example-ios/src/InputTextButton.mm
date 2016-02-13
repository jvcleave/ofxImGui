//
//  InputTextButton.m
//  ImGuiTest
//
//  Created by Jason Van Cleave on 2/12/16.
//
//

#include "InputTextButton.h"



@implementation TextField

- (void)textFieldEditingChanged:(id)sender
{
    
    TextField* tf = (TextField *)sender;
    inputTextButton->onTextEdit([tf.text UTF8String]);
}
@end


void InputTextButton::onTextEdit(string s)
{
    text = [textField.text UTF8String];
    if(text.empty())
    {
        //text = [textField.placeholder UTF8String];
    }
}
InputTextButton::InputTextButton()
{
    textField = NULL;
    hasText = false;
    text = "";
};

void InputTextButton::setup(string initialString)
{
    text = initialString;
    textField = [[TextField alloc] initWithFrame:CGRectMake(0, 0, 100, 20)];
    textField.text = [NSString stringWithUTF8String:text.c_str()];
    textField.placeholder = textField.text;
    textField.hidden = YES;
    textField->inputTextButton  = this;

    
    [textField addTarget:textField
                    action:@selector(textFieldEditingChanged:)
                    forControlEvents:UIControlEventEditingChanged];
    
    
    [ofxiOSGetGLParentView() addSubview:textField];
    
}
void InputTextButton::draw()
{
    if(ImGui::Button(text.c_str()))
    {
        if(![textField isFirstResponder])
        {
            [textField becomeFirstResponder];
        }
    }
    textField.text = [NSString stringWithUTF8String:text.c_str()];
}

