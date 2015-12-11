ofxImgui
========

![Screenshot](Screenshot.png)


```cpp
void ofApp::draw()
{
  m_gui.begin();

    static bool show_another_window = true;
    static bool show_test_window = true;

    if(show_another_window)
    {
      ui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
      ui::Begin("Another Window", &show_another_window);
      ui::Text("Hello");
      ui::End();
    }

    if(show_test_window)
    {
      ui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ui::ShowTestWindow(&show_test_window);
    }

  m_gui.end();
}
```
