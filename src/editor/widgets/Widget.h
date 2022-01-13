#ifndef RAYTRACER_WIDGET_H
#define RAYTRACER_WIDGET_H

namespace Editor
{
    class Widget
    {
    public:
        Widget(const char* title) : m_title(title) {}

        virtual void draw() = 0;

    protected:
        const char *m_title;
        bool m_visible = false;
    };
}

#endif //RAYTRACER_WIDGET_H
