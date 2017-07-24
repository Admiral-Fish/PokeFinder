<<<<<<< Updated upstream
#include "Objects/NatureLock.hpp"
#include "Objects/EncounterSlot.hpp"
#include <iostream>
=======
#include <wx/app.h>
#include <wx/event.h>
#include "MainFrame.h"
#include <wx/image.h>
>>>>>>> Stashed changes

// Define the MainApp
class MainApp : public wxApp
{
<<<<<<< Updated upstream
    std::cout << EncounterSlot::KSlot(0x12345678, Encounter::Wild) << std::endl;
}
=======
public:
    MainApp() {}
    virtual ~MainApp() {}

    virtual bool OnInit() {
        // Add the common image handlers
        wxImage::AddHandler( new wxPNGHandler );
        wxImage::AddHandler( new wxJPEGHandler );

        MainFrame *mainFrame = new MainFrame(NULL);
        SetTopWindow(mainFrame);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
>>>>>>> Stashed changes
