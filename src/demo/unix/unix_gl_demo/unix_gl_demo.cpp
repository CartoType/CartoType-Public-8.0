#define GLFW_INCLUDE_ES2

#include <GLFW/glfw3.h>
#include <cartotype.h>
#include <stdexcept>

class MapWindow
    {
    public:
    MapWindow();
    ~MapWindow();
    bool Draw();

    private:
    static void HandleKeyStroke(GLFWwindow* aWindow,int aKey,int aScancode,int aAction,int aMods);
    static void HandleCursor(GLFWwindow* aWindow,double aX,double aY);
    static void HandleCursorEntry(GLFWwindow* aWindow,int aEntered);
    static void HandleMouseButton(GLFWwindow* aWindow,int aButton,int aAction,int aMods);
    static void HandleScroll(GLFWwindow* aWindow,double aX,double aY);
    void KeyStroke(int aKey,int aScancode,int aAction,int aMods);
    void LeftButtonDown(double aX,double aY);
    void LeftButtonUp(double aX,double aY);
    void RightButtonDown(double aX,double aY);
    void RightButtonUp(double aX,double aY);
    void StopDragging();
    void PanToDraggedPosition();
    void MouseMove(double aX,double aY);
    void MouseEntry(bool aEntered);
    void MouseWheel(double aX,double aY);
    void CalculateAndDisplayRoute();

    GLFWwindow* m_window = nullptr;
    std::unique_ptr<CartoType::Framework> m_framework;
    std::unique_ptr<CartoType::MapRenderer> m_map_renderer;
    bool m_mouse_in_window = false;
    bool m_map_drag_enabled = false;
    CartoType::PointFP m_map_drag_anchor;
    CartoType::PointFP m_map_drag_anchor_in_map_coords;
    CartoType::PointFP m_map_drag_offset;
    CartoType::PointFP m_route_start;
    CartoType::PointFP m_route_end;
    };

MapWindow::MapWindow()
    {
    m_window = glfwCreateWindow(1024,1024,"CartoType for Unix",nullptr,nullptr);
    if (!m_window)
        throw std::runtime_error("could not create GLFW window");
    glfwMakeContextCurrent(m_window);

    CartoType::Result error;
    m_framework = CartoType::Framework::New(error,"../../../../src/test/data/ctm1/santa-cruz.ctm1","../../../../style/standard.ctstyle","../../../../font/DejaVuSans.ttf",1024,1024);
    m_map_renderer = std::make_unique<CartoType::MapRenderer>(*m_framework);

    double x = 0, y = 0;
    glfwGetCursorPos(m_window,&x,&y);
    m_mouse_in_window = x >= 0 && x < 1024 && y >= 0 && y < 1024;

    glfwSetWindowUserPointer(m_window,this);
    glfwSetKeyCallback(m_window,HandleKeyStroke);
    glfwSetCursorPosCallback(m_window,HandleCursor);
    glfwSetCursorEnterCallback(m_window,HandleCursorEntry);
    glfwSetMouseButtonCallback(m_window,HandleMouseButton);
    glfwSetScrollCallback(m_window,HandleScroll);
    }

bool MapWindow::Draw()
    {
    if (glfwWindowShouldClose(m_window))
        return false;
    m_map_renderer->Draw();
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);
    return true;
    }

MapWindow::~MapWindow()
    {
    glfwDestroyWindow(m_window);
    }

void MapWindow::HandleKeyStroke(GLFWwindow* aWindow,int aKey,int aScancode,int aAction,int aMods)
    {
    auto map_window = (MapWindow*)glfwGetWindowUserPointer(aWindow);
    map_window->KeyStroke(aKey,aScancode,aAction,aMods);
    }

void MapWindow::HandleCursor(GLFWwindow* aWindow,double aX,double aY)
    {
    auto map_window = (MapWindow*)glfwGetWindowUserPointer(aWindow);
    map_window->MouseMove(aX,aY);
    }

void MapWindow::HandleCursorEntry(GLFWwindow* aWindow,int aEntered)
    {
    auto map_window = (MapWindow*)glfwGetWindowUserPointer(aWindow);
    map_window->MouseEntry(aEntered != 0);
    }

void MapWindow::HandleMouseButton(GLFWwindow* aWindow,int aButton,int aAction,int aMods)
    {
    auto map_window = (MapWindow*)glfwGetWindowUserPointer(aWindow);
    double x = 0, y = 0;
    glfwGetCursorPos(aWindow,&x,&y);
    if (aButton == GLFW_MOUSE_BUTTON_LEFT)
        {
        if (aAction == GLFW_PRESS)
            map_window->LeftButtonDown(x,y);
        else if (aAction == GLFW_RELEASE)
            map_window->LeftButtonUp(x,y);
        }
    else if (aButton == GLFW_MOUSE_BUTTON_RIGHT)
        {
        if (aAction == GLFW_PRESS)
            map_window->RightButtonDown(x,y);
        else if (aAction == GLFW_RELEASE)
            map_window->RightButtonUp(x,y);
        }
    }

void MapWindow::HandleScroll(GLFWwindow* aWindow,double aX,double aY)
    {
    auto map_window = (MapWindow*)glfwGetWindowUserPointer(aWindow);
    map_window->MouseWheel(aX,aY);
    }

void MapWindow::KeyStroke(int aKey,int aScancode,int aAction,int aMods)
    {
    if (aAction == GLFW_RELEASE)
        return;
    switch (aKey)
        {
        case GLFW_KEY_I: m_framework->Zoom(1.05); break;
        case GLFW_KEY_O: m_framework->Zoom(1.0 / 1.05); break;
        case GLFW_KEY_P: m_framework->SetPerspective(!m_framework->Perspective()); break;
        case GLFW_KEY_RIGHT: m_framework->Pan(32,0); break;
        case GLFW_KEY_LEFT: m_framework->Pan(-32,0); break;
        case GLFW_KEY_UP: m_framework->Pan(0,-32); break;
        case GLFW_KEY_DOWN: m_framework->Pan(0,32); break;
        }
    }

void MapWindow::LeftButtonDown(double aX,double aY)
    {
    m_map_drag_enabled = true;
    m_map_drag_anchor.X = aX;
    m_map_drag_anchor.Y = aY;
    m_map_drag_anchor_in_map_coords = m_map_drag_anchor;
    m_framework->ConvertPoint(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);
    }

void MapWindow::LeftButtonUp(double aX,double aY)
    {
    m_map_drag_offset.X = aX - m_map_drag_anchor.X;
    m_map_drag_offset.Y = aY - m_map_drag_anchor.Y;
    StopDragging();
    }

void MapWindow::RightButtonDown(double aX,double aY)
    {
    }

void MapWindow::RightButtonUp(double aX,double aY)
    {
    CartoType::PointFP p(aX,aY);
    m_framework->ConvertPoint(p.X,p.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);
    if (glfwGetKey(m_window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_route_end = p;
    else
        m_route_start = p;
    CalculateAndDisplayRoute();
    }

void MapWindow::StopDragging()
    {
    m_map_drag_enabled = false;
    PanToDraggedPosition();
    m_map_drag_offset = { 0, 0 };
    }

void MapWindow::PanToDraggedPosition()
    {
    CartoType::PointFP to = m_map_drag_anchor;
    to += m_map_drag_offset;
    m_framework->Pan(m_map_drag_anchor.Rounded(),to.Rounded());
    }

void MapWindow::MouseMove(double aX,double aY)
    {
    if (m_map_drag_enabled)
        {
        m_map_drag_offset.X = aY - m_map_drag_anchor.X;
        m_map_drag_offset.Y = aY - m_map_drag_anchor.Y;
        m_framework->Pan(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Map,
                         aX,aY,CartoType::CoordType::Display);
        m_map_drag_offset = CartoType::Point(0,0);
        m_map_drag_anchor.X = aX;
        m_map_drag_anchor.Y = aY;
        m_map_drag_anchor_in_map_coords = m_map_drag_anchor;
        m_framework->ConvertPoint(m_map_drag_anchor_in_map_coords.X,m_map_drag_anchor_in_map_coords.Y,CartoType::CoordType::Display,CartoType::CoordType::Map);
        }
    }

void MapWindow::MouseEntry(bool aEntered)
    {
    m_mouse_in_window = aEntered;
    }

void MapWindow::MouseWheel(double aX,double aY)
    {
    auto delta = aY;
    if (delta == 0)
        return;

    // If the mouse pointer is in the window, use it as the centre of zoom or rotation.
    bool set_centre = m_mouse_in_window;
    CartoType::PointFP p;
    glfwGetCursorPos(m_window,&p.X,&p.Y);

    // If the left shift key is pressed, rotate the map.
    if (glfwGetKey(m_window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
        auto rotation = delta * 6;
        if (set_centre)
            m_framework->RotateAt(rotation,p.X,p.Y,CartoType::CoordType::Display);
        else
            m_framework->Rotate(rotation);
        return;
        }

    double zoom = sqrt(2);
    int zoom_count = delta / 4;
    if (zoom_count == 0)
        zoom_count = delta >= 0 ? 1 : -1;
    zoom = pow(zoom,zoom_count);

    if (set_centre)
        m_framework->ZoomAt(zoom,p.X,p.Y,CartoType::CoordType::Display);
    else
        m_framework->Zoom(zoom);
    }

void MapWindow::CalculateAndDisplayRoute()
    {
    if (m_route_start == CartoType::PointFP() || m_route_end == CartoType::PointFP())
        return;
    m_framework->StartNavigation(m_route_start.X,m_route_start.Y,CartoType::CoordType::Map,m_route_end.X,m_route_end.Y,CartoType::CoordType::Map);
    }

int main(void)
    {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Create a Map window.
    MapWindow map_window;

    /* Draw continuously until the user closes the window */
    while (map_window.Draw())
        {

        /* Poll for and process events */
        glfwPollEvents();
        }

    glfwTerminate();
    return 0;
    }
