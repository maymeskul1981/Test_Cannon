#pragma once

#include <vector>

//------------------------ Target Class --------------------------
// ����� ������
class Target {
public:
	// ����������� � ���������� ��������� �������
	Target(FPoint position, float speed);
	~Target();
	void Draw();
	void Update(float dt);

	// �������� ������� ������
	FPoint getPosition();
	FPoint getSpeedVector();
	IRect getRect();
	void setSpeedVector(FPoint speedVector);
	void setPosition(float x, float y);
	float getScaleTarget();
	float getMassTarget();	
	bool hitOver();

	// ��������� ��������� � ������
	bool chekHit(FPoint posArrow, EffectsContainer &effCont);
	// �������� �������� ���������
	bool getHit();
	
private:
	// �������� ������
	Render::Texture *_trg;
	// ������� ������
	FPoint _position;
	// �������� 
	float _speed;
	//�����
	float _mass;
	// ������� ��������
	float _scale;
	// ������ ��������
	FPoint _speedVector;
	// ���� "���������"
	bool _hitting;
	int _life;
	
	// ��������� � ���������
	EffectsContainer	_effCont;
	
};

//-------------------------- Targets Class ----------------
// ����� ���������� ��������

class TargetsController
{
public:
	// ����������� � ����������� ���������� ������� � ����������� �� �������
	TargetsController(int count, float timeLimit, float speed);	// �������������	 
	void Init();
	void Draw();
	void Update(float dt);
	// �������� ��������� ����� � ������
	bool ChekingHits(FPoint posBullet, EffectsContainer &effCont);

	// ������� ������� �������
	void Clear();
	// �������� ���������� ������� � ������� ������
	int GetCountTargetNow();	
	Target* TargetsController::CheckNewTarget();

private:
	// ������ � ��������
	std::vector <Target *> _targets;
	// ����������
	int _count;
	float _speed;
	// ����������� �� �������
	float _timeLimit;
};