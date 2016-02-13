//
//  InputTextButton.m
//  ImGuiTest
//
//  Created by Jason Van Cleave on 2/12/16.
//
//

#include "InputTextButton.h"

@implementation TextFieldDelegate

- (void)textFieldEditingChanged:(id)sender
{
    
    UITextField* textField = (UITextField *)sender;
    inputTextButton->onTextEdit([textField.text UTF8String]);
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
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
    text = "";
    textFieldDelegate = [[TextFieldDelegate alloc] init];
};

void InputTextButton::setup(string initialString)
{
    text = initialString;
    
    textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 100, 20)];
    textField.text = [NSString stringWithUTF8String:text.c_str()];
    textField.placeholder = textField.text;
    textField.hidden = YES;
    textField.delegate = textFieldDelegate;
    textFieldDelegate->inputTextButton  = this;
    
    [textField addTarget:textFieldDelegate
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

