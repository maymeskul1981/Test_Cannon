#include "stdafx.h"
#include "Cannon.h"
#include "TestWidget.h"
#include "TargetsController.h"

Bullet::Bullet(float scale, float speed, float angle, EffectsContainer &effCont) {
	// ������
	_eff = effCont.AddEffect("FireWorks");
	// ��������� �������� ������� �� ���������� ������������
	_scale = scale;
	_speed = speed;
	_angle = angle;
	// �������������
	Init();
}

void Bullet::Init() {	
	_kTime = 10;
	// �������� �������� ������
	_bullet = Core::resourceManager.Get<Render::Texture>("Bullet");
	// ��������� ��������� �������
	_position = IPoint(TestWidget::WINDOW_WIDTH / 2, 10);
	_isHit = false;

	// ���������� ������� ������� ����������� � ������ �� ����
	_eff->posX = _position.x;
	_eff->posY = _position.y;
	_eff->Reset();
}

Bullet::~Bullet() {

}

FPoint Bullet::getPosition() {
	return _position;
}

// ���������� ����� "���������"
void Bullet::setHit(bool hit) {
	_isHit = hit;
	// ��������� ������ � ����������� ������
	if (_eff) {
	_eff->Finish();
	_eff = NULL;
	}
}

void Bullet::Draw() {
	// ��������� ��������� ��������
	_effCont.Draw();
	
	// ���� ��� ��������� �� ����	
	if (!_isHit) {
		Render::device.PushMatrix();
		// �������� �� ������� �������
		Render::device.MatrixTranslate(math::Vector3(_position.x, _position.y, 0));
		// ������������ �� ������ ����
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		// �������� ������ ��������
		IRect texRect = _bullet->getBitmapRect();
		FRect rect(texRect), uv(0, 1, 0, 1);
		_bullet->TranslateUV(rect, uv);

		// ������������� ������� ��������
		Render::device.MatrixScale(_scale);
		// �������� �� "����������" ������
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height, 0.f));

		// ���������� ��������
		_bullet->Bind();
		Render::DrawQuad(rect, uv);
		Render::device.PopMatrix();
	}
}

void Bullet::Update(float dt) {
	// ������������ ��������� ������
	_position.x += _speed * math::cos((_angle + 90) * math::PI / 180) * dt * _kTime;
	_position.y += _speed * math::sin((_angle + 90) * math::PI / 180) * dt * _kTime;

	_effCont.Update(dt);
	// ���� ��������� �� ����
	if (!_isHit) {
		// �������� ������
		_eff->posX = _position.x;
		_eff->posY = _position.y;
	}
}

bool Bullet::getHit() {
	return _isHit;
}

void Bullet::KillEffect() {
	if (_eff) {
		_eff->Kill();
		_eff = NULL;
	}
}

// --------------------------- Gun Class -------------------
// ����� �����

Cannon::Cannon() {
	Init();
}

void Cannon::Init() {
	// ��������� �������� �����
	_cannon = Core::resourceManager.Get<Render::Texture>("Cannon");
	// ������� ������� ����������� ��� ������� �������� ����� � ��������
	_scale = 0.2;
	// ��������� ���� 0 ��������
	_angle = 0;
	// ������� ����� �� �������� ������
	_posCannon = IPoint(TestWidget::WINDOW_WIDTH / 2, 50);
}

void Cannon::Draw() {
	// ������ ������, ���� ��� ����
	for (unsigned i = 0; i < _bullets.size(); i++) {
		_bullets[i]->Draw();
	}

	// ������ ����� � �������� ������������ ��������� �����
	Render::device.PushMatrix();

	Render::device.MatrixTranslate(math::Vector3(_posCannon.x, _posCannon.y, 0));
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	IRect texRect = _cannon->getBitmapRect();
	FRect rect(texRect), uv(0, 1, 0, 1);
	_cannon->TranslateUV(rect, uv);

	Render::device.MatrixScale(_scale);
	Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

	_cannon->Bind();

	Render::DrawQuad(rect, uv);

	Render::device.PopMatrix();
}

void Cannon::DeletBullet(unsigned n) {
	// �� ������� ���� �� ������
	_bullets[n]->KillEffect();
	delete _bullets[n];
	// � �� �������
	_bullets.erase(_bullets.begin() + n);
}

void Cannon::Update(float dt) {
	unsigned i = 0;
	IRect texRect = _cannon->getBitmapRect();
	// ��������� ��� ������ ������� ����	
	while (i < _bullets.size()) {
		_bullets[i]->Update(dt);
		// ���������, ���� ������ ����� �� ������� ������
		FPoint aPoint = _bullets[i]->getPosition();		
		if ((aPoint.y > (TestWidget::WINDOW_HEIGHT + texRect.height)) ||
			(aPoint.y < (-texRect.height)) ||
			(aPoint.x > (TestWidget::WINDOW_WIDTH + texRect.height)) ||
			(aPoint.x < (-texRect.height))) {
			// �� ������� ������ �� ������
			DeletBullet(i);
		}
		else {
			i++;
		}
	}
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	// ����������� ���� ������� ����� � ��������
	if ((mouse_pos.x - _posCannon.x) != 0) {
		_angle = math::atan((mouse_pos.y - _posCannon.y), (mouse_pos.x - _posCannon.x)) * 180.0f / math::PI - 90;
	}
}

// ������ ����� ������ �����
bool Cannon::MouseDown(const IPoint& mouse_pos, int speed, EffectsContainer &effCont) {	
	// ������� ����� ����
	_bullets.push_back(new Bullet (_scale, speed, _angle, effCont));
	return false;
}

// �������� ��������� � ������
bool Cannon::CheckTargetsHit(TargetsController *targetsRuf, EffectsContainer &effCont) {
	
	bool status = false;
	for (int i = _bullets.size() - 1; i >= 0; i--) {
		// ���� ������ ��� �� �������� �� � ���� ������
		if (!_bullets[i]->getHit()) {
			// � ���� ��� ������ � ������
			if (targetsRuf->ChekingHits(_bullets[i]->getPosition(), effCont)) {
				// �� ������������� ���� "���������"
				_bullets[i]->setHit(true);
				status = true;
			}
		}
	}	
	return status;
}


// ������� ��� ������
void Cannon::ClearBullets() {
	
	for (int i = _bullets.size() - 1; i >= 0; i--) {
		_bullets[i]->KillEffect();
		delete _bullets[i];
	}
	_bullets.clear();	
}


int Cannon::getCountBullet() {
	return _bullets.size();
}