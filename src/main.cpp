#include <iostream>
#include <thread>

#include <opencv2/opencv.hpp>
#include <gtkmm.h>

#include "SkelRecorder.h"

void worker()
{
    SkelRecorder recorder;
    recorder.init();
    while (true);
}

int main()
{
    auto app = Gtk::Application::create();
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("../src/mainwindow.glade");
    Gtk::Window* pWindow;
    builder->get_widget("mainWindow", pWindow);
    app->run(*pWindow);
}
