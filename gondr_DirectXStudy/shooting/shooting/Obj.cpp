#include "Obj.h"

float Obj::GetAngle()
{
    return m_fAngle;
}

void Obj::SetAngle(float value)
{
    m_fAngle = value;
}

FPOINT Obj::GetCenter()
{
    return FPOINT{
        m_Pos.x + m_Size.width / 2,
        m_Pos.y + m_Size.height / 2
    };
}
