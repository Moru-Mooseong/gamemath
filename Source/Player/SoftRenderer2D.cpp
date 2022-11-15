
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// 격자를 그리는 함수
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// 좌측 하단에서부터 격자 그리기
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// 그리드가 시작되는 좌하단 좌표 값 계산
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// 게임 오브젝트 목록

// 최초 씬 로딩을 담당하는 함수
void SoftRenderer::LoadScene2D()
{
	// 최초 씬 로딩에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();

}

// 게임 로직과 렌더링 로직이 공유하는 변수
Vector2 currentPosition(100.f, 30.f);	//물체의 위치를 보관하는 변수 (초기화값 100x, 100y)


// 게임 로직을 담당하는 함수
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// 게임 로직의 로컬 변수
	static float moveSpeed = 100.f;	//프레임당 이동속도를 지정
	
	Vector2 inputVector = Vector2(input.GetAxis(InputAxis::XAxis), (input.GetAxis(InputAxis::YAxis))); //X축과 Y축 입력을 결합해 입력벡터를 생성한다.
	Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;		//이동속도와 프레임의 경과시간 InDeltaTime을 곱하여 해당 프레임에서 이동할 길이를 계산한 후 이를 입력 벡터에 곱하고 그 결과를 저장한다.

	currentPosition += deltaPosition;		//물체의 최종 상태를 결정한다.

}

// 렌더링 로직을 담당하는 함수
void SoftRenderer::Render2D()
{
	// 렌더링 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 배경에 격자 그리기
	DrawGizmo2D();

	// 렌더링 로직의 로컬 변수
	
	//밝을 회색의 선을 사용해 평행한 백터를 표현
	static float lineLength = 500.f;										//13.벡터 currentPosition과 평행한 벡터를 표현하도록 길이를 충분히 크게 지정한다.
	Vector2 lineStart = currentPosition * lineLength;						//14. 벡터 currentPosition과 평팽한 선의 시작점을 계산한다.
	Vector2 lineEnd = currentPosition * -lineLength;						//14. 벡터 currentPosition과 평행한 선의 끝점을 계산한다.
	r.DrawLine(lineStart, lineEnd, LinearColor::LightGray);					//15. 렌더러에서 제공하는 DrawLine함수를 사용해 밝은 회색으로 평행한 선을 표현한다.

	//벡터를 파란색 픽셀로 표현
	r.DrawPoint(currentPosition, LinearColor::Blue);						//16. 파란 점으로 벡터 CurrentPosition을 화면에 찍는다. 점이 잘 보이도록 8방향에 대해서도 픽셀을 찍는다.
	r.DrawPoint(currentPosition+Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2(1.f, -1.f), LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2(1.f, -1.f), LinearColor::Blue);

	//벡터의 현재 좌표를 우상단에 출력
	r.PushStatisticText("Coordinate : " + currentPosition.ToString());		//17. 우상단에 벡터 currentPosition의 좌표를 출력한다.
}

// 메시를 그리는 함수
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// 삼각형을 그리는 함수
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
