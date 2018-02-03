
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <QApplication>
#include "MainWindow.h"

#include <random>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ext.hpp>

int main(int argc, char *argv[])
{
    // create an OpenGL format specifier
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples( 4 );
    #if defined( DARWIN )
      // at present mac osx Mountain Lion only supports GL3.2
      // the new mavericks will have GL 4.x so can change
      format.setMajorVersion( 4 );
      format.setMinorVersion( 2 );
    #else
      // with luck we have the latest GL version so set to this
      format.setMajorVersion( 4 );
      format.setMinorVersion( 3 );
    #endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile( QSurfaceFormat::CoreProfile );
    // now set the depth buffer to 24 bits
    format.setDepthBufferSize( 24 );

    // this will set the format for all widgets
    QSurfaceFormat::setDefaultFormat( format );
    // make an instance of the QApplication
    QApplication a( argc, argv );
    // Create a new MainWindow
    MainWindow w;
    // show it
    w.show();
    // hand control over to Qt framework
    return a.exec();
}
