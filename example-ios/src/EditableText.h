#pragma once

#include "ofxiOSExtras.h"
#include "ofxImGui.h"


class EditableText
{
    
public:
    EditableText()
    {
        textField = NULL;
        inputTextValue = "";
        
    };
    
    void setup(string initialString)
    {
        inputTextValue = initialString;
        textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 100, 20)];
        [textField setPlaceholder:[NSString stringWithUTF8String:inputTextValue.c_str()]];
        textField.hidden = YES;
        [ofxiOSGetGLParentView() addSubview:textField];
    }
    
    void begin()
    {
        
        if(ImGui::GetIO().WantTextInput)
        {
            [textField becomeFirstResponder];
        }
        
        inputTextValue = [textField.text UTF8String];
        
        
    }
    
    void end()
    {
        textField.text = [NSString stringWithUTF8String:inputTextValue.c_str()];
    }
    
    char* getText()
    {
        return (char *)inputTextValue.c_str();
    }
    
    UITextField* textField;
    string inputTextValue;
    
};
