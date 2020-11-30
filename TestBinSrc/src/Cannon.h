#pragma once
#include "TargetsController.h"
#include <vector>

class Bullet
{
public:
	// ����������� � ����������� - ����������� �������� ��������, 
	// �������� ������ ������� � ���� ������� ��������
	Bullet(float scale, float speed, float angle, EffectsContainer &effCont);
	// ����������
	~Bullet();

	void Draw();
	void Update(float dt);

	// ���������� �������� ����� "���������"
	void setHit(bool hit);
	// �������� �������� ����� "���������"
	bool getHit();
	// �������� ������� �������
	FPoint getPosition();
	// ������� ������
	void KillEffect();

private:
	void Init();

private:
	// �������� ������
	Render::Texture *_bullet;
	// �������
	FPoint _position;
	// ���� �������� ��������
	float _angle;
	// ������� ��������
	float _scale;
	// �������� ������� (���� / ���)
	float _speed;
	// ���� "���������"
	bool _isHit;
	// ����������� �������
	int _kTime;

	ParticleEffectPtr _eff;
	EffectsContainer _effCont;
};

// -------------------------- Gun Class -----------------------------
// ����� "������". ����� ����� ������
class Cannon
{
public:
	Cannon();
	void Draw();
	void Update(float dt);
	// ������� ������ ����
	bool MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont);
	// �������� ���������� ���������� �����
	//int getCountBullet();
	// ������� ��� ������
	void ClearBullets();
	// �������� ��������� � ������
	bool CheckTargetsHit(TargetsController *targetsRuf, EffectsContainer &effCont);
	// ������� ���� �� �������
	void DeletBullet(unsigned n);
	int Cannon::getCountBullet();

private:
	void Init();

private:
	// �������� �����
	Render::Texture *_cannon;
	// ��������� �����
	IPoint _posCannon;
	// ������� ��������
	float _scale;
	// ���� �������� ��������
	float _angle;
	// ������ � ������
	std::vector<Bullet *> _bullets;
};