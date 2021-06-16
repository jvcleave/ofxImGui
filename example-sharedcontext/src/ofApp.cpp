#include "ofApp.h"

struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of
            // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
            // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
            // within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
            // on your side is recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display
            // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
            // it possible (and would be recommended if you want to search through tens of thousands of entries).
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};

//--------------------------------------------------------------
void ofApp::setup()
{
    // First call to setup() for your main application menu (master).
    // Any next call to setup() will setup ofxImGui as a slave.
    ofAppGui.setup(nullptr, false, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable , true);

    // There's 2 ways of achieving multi context.
    // By default, it's automatically enabled when a 2nd ofxImGui instance is created in the same ofApp.
    // You can also force the shared mode, needed when the shared code cannot be modified

    // This all is needed for privateAddonClass, which "say" we cannot modify, and hasn't specially been build for use in shared contexts
    ofAppGui.setSharedMode(true); // Force shared context

    // Setup addon guis in shared mode
    myAddonObject.setup( ofAppGui ); // Preferred method : share the menu handle; when you can modify myAddonClass
    privateAddonObject.setup(); // Calls Gui::setup() after `setSharedMode`, which grabs a single
}

//--------------------------------------------------------------
void ofApp::drawImGui() 
{
    ofAppGui.begin(); // <-- First call = Normal behaviour in sharedMode
	{
        ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);


        // Position the Gui only the firt time, after which it's reset to the last used layout.
        ImGui::SetNextWindowPos(ImVec2(0,20), ImGuiCond_FirstUseEver );
        ImGui::SetNextWindowSize(ImVec2(260,ofGetHeight()-20), ImGuiCond_FirstUseEver );
        ImGui::Begin("ofApp gui");
        ImGui::Separator();
        ImGui::Text("Background color :");
        ImGui::ColorPicker4("##picker", (float*)&ofAppColor);
		ImGui::End();

        static bool bShowDemoWindow = false;
        static bool bShowLogWindow = false;
        ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("Main")){
            ImGui::Checkbox("Show demo window", &bShowDemoWindow);
            ImGui::Checkbox("Show logs", &bShowLogWindow);
            ImGui::Separator();

            // Add default menu entries
            ImGui::MenuItem( "Open" );
            ImGui::MenuItem( "Save" );
            ImGui::MenuItem( "Quit" );

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if(bShowDemoWindow) ImGui::ShowDemoWindow();

        if(bShowLogWindow){
            static ExampleAppLog log;

            // For the demo: add a debug button _BEFORE_ the normal log window contents
            // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
            // Most of the contents of the window will be added by the log.Draw() call.
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Example: Log", &bShowLogWindow);
            if (ImGui::SmallButton("[Debug] Add 5 entries"))
            {
                static int counter = 0;
                const char* categories[3] = { "info", "warn", "error" };
                const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
                for (int n = 0; n < 5; n++)
                {
                    const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                    const char* word = words[counter % IM_ARRAYSIZE(words)];
                    log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                        ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                    counter++;
                }
            }
            ImGui::End();

            // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
            log.Draw("Example: Log", &bShowLogWindow);
        }
	}
    ofAppGui.end(); // <-- Does nothing with sharedMode on
}

//--------------------------------------------------------------
void ofApp::draw() 
{
    // oF drawing
    ofPushStyle();
    ofClear(ofAppColor);
    privateAddonObject.draw();
    myAddonObject.draw();
    ofPopStyle();

    // GUI drawing

	// instance 1
    this->drawImGui();

    // instance 2 - Plugin code that was made for sharing context
    ImGui::PushID("##SomeUniqueSandboxHash"); // <-- if you want to ensure a sandbox between both (fails with sharedMode off)
	myAddonObject.drawImGui();
    ImGui::PopID(); // <-- If you want to ensure a sandbox between both

    // Instance 3 - Plugin code that was not mode for sharing context
    privateAddonObject.drawImGui();

    // Manually render once all GUI items have been submitted
    ofAppGui.draw(); //    <-- Also calls ofAppGui.end()
}
