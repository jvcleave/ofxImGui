//
//  EditableText.m
//  ImGuiTest
//
//  Created by Jason Van Cleave on 2/12/16.
//
//

#include "EditableText.h"



@implementation TextField

- (void)deleteBackward
{
    [super deleteBackward];
    editableText->onTextDelete();
    
}

- (void)insertText:(NSString *)text
{
    
    [super insertText:text];
    editableText->onInsertText([text UTF8String]);

}

- (BOOL)hasText
{
    BOOL result = [super hasText];
    if (result)
    {
        editableText->hasText = true;
    }else
    {
        editableText->hasText = false;
    }
    

    return result;
}

- (void)textFieldEditingChanged:(id)sender
{
    
    TextField* tf = (TextField *)sender;
    editableText->onTextEdit([tf.text UTF8String]);
}
@end

void EditableText::onInsertText(string text)
{
    ofLogVerbose(__func__) << "text: " << text;
}

void EditableText::onTextDelete()
{
    ofLogVerbose(__func__) << "";
    ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]] = true;
}

void EditableText::onTextEdit(string s)
{
    bool keysDownValue = ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Backspace]];
    ofLogVerbose(__func__) << "s: " << s << " keysDownValue: " << keysDownValue;
    unsigned short lastCharacter = (unsigned short)s.back();
    ImGui::GetIO().AddInputCharacter(lastCharacter);
}
EditableText::EditableText()
{
    textField = NULL;
    hasText = false;
    inputTextValue = "";
    internalBuffer = NULL;
};

void EditableText::setup(string initialString)
{
    inputTextValue = initialString;
    textField = [[TextField alloc] initWithFrame:CGRectMake(0, 0, 100, 20)];
    NSString* initialText = [NSString stringWithUTF8String:inputTextValue.c_str()];
    textField.text = initialText;
    textField.placeholder = textField.text;
    textField.hidden = NO;
    textField->editableText  = this;
    //textField.myDelegate = self;

    
    [textField addTarget:textField
                    action:@selector(textFieldEditingChanged:)
                    forControlEvents:UIControlEventEditingChanged];
    
    
    [ofxiOSGetGLParentView() addSubview:textField];
    
}

void EditableText::begin()
{
    
    if(ImGui::GetIO().WantTextInput)
    {
        [textField becomeFirstResponder];
    }
    
    inputTextValue = [textField.text UTF8String];
}

void EditableText::end()
{
    if(internalBuffer)
    {
        internalBuffer = (char *)inputTextValue.c_str();
    }
    textField.text = [NSString stringWithUTF8String:inputTextValue.c_str()];
}

char* EditableText::getText()
{
    if(internalBuffer)
    {
        return internalBuffer;
    }
    return (char *)inputTextValue.c_str();
}
