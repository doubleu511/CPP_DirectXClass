#include "Bullet.h"

Bullet::Bullet(FPOINT pos, OBJECTSIZE size, const TCHAR* filename) : Obj(pos, size), m_Rect{}
{
    m_fAngle = 0;
    m_ImageBullet.Load(filename);
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
    m_Size.width = m_ImageBullet.GetBitmapInfo().bmWidth;
    m_Size.height = m_ImageBullet.GetBitmapInfo().bmHeight;

    m_Rect.left = (LONG)m_Pos.x;
    m_Rect.right = static_cast<LONG>(m_Rect.left + m_Size.width);
    m_Rect.top = (LONG)m_Pos.y;
    m_Rect.bottom = static_cast<LONG>(m_Rect.top + m_Size.height);
}

void Bullet::Update(float deltaTime)
{

}

void Bullet::Render(HDC hdc, float deltaTime)
{
    m_ImageBullet.DrawBitmap(hdc, (int)m_Pos.x, (int)m_Pos.y, (int)(m_Size.width), (int)(m_Size.height));
}

void Bullet::Release()
{
    m_ImageBullet.Release();
}

bool Bullet::IsCollision(shared_ptr<Player> player)
{
    // 이 부분이 수행평가가 될꺼다.

    Vector2 dist = { GetCenter().x - player->GetCenter().x , GetCenter().y - player->GetCenter().y };

    Vector2 topPos = { GetCenter().x, static_cast<double>(m_Rect.top) };
    topPos.Rotate(GetAngle());

    Vector2 playerTopPos = { player->GetCenter().x, static_cast<double>(player->GetRect()->top) };
    playerTopPos.Rotate(GetAngle());

    Vector2 rightPos = { static_cast<double>(m_Rect.right), GetCenter().y };
    rightPos.Rotate(GetAngle());

    Vector2 playerRightPos = { static_cast<double>(player->GetRect()->right), player->GetCenter().y};
    playerRightPos.Rotate(GetAngle());

    Vector2 vec[4] = {
        { topPos.x - GetCenter().x, topPos.y - GetCenter().y },
        { playerTopPos.x - player->GetCenter().x, playerTopPos.y - player->GetCenter().y },
        { rightPos.x - GetCenter().x, rightPos.y - GetCenter().y },
        { playerRightPos.x - player->GetCenter().x, playerRightPos.y - player->GetCenter().y },
    };

    for (int i = 0; i < 4; i++) {
        double sum = 0;
        Vector2 normal = vec[i].Normal();
        for (int j = 0; j < 4; j++) {
            sum += abs(vec[j].Dot(normal));
        }
        if (abs(dist.Dot(normal)) > sum) {
            return false;
        }
    }
    return true;
}

RECT* Bullet::GetRect()
{
    return &m_Rect;
}
