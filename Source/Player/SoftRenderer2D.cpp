
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
Vector2 currentPosition(100.f, 100.f);

// 게임 로직을 담당하는 함수
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// 게임 로직의 로컬 변수
	static float moveSpeed = 100.f;

	Vector2 inputVector = Vector2(input.GetAxis(InputAxis::XAxis), input.GetAxis(InputAxis::YAxis)).GetNormalize();		//1. GetNormalize() 함수를 사용하여 입력벡터의 크기를 항상 1로 정규화한다.
	Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;

	// 물체의 최종 상태 설정
	currentPosition += deltaPosition;
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
	static float radius = 50.f;					//2. 그릴 원의 반지름을 50으로 설정한다.
	static std::vector<Vector2> circles;		//3. 원을 구성하는 점을 보관하기 위한 자료구조로 vector를 선택하고 이를 circles로 선언한다. 이때 한번 생성한 변수는 계속 유지될 수 있도록 변수는 static으로 선언한다.
	
	//최초에 한 번 반지름보다 긴 벡터를 모아 컨테이너에 담는다.
	if (circles.empty())											//4. circles 자료구조가 비어있은 경우에만 원을 구성하는 점을 채워넣는다.
	{
		for (float x = -radius; x <= radius; x++)					//5. 반지름 정보를 활용해 원이 포함된 사각형 영역을 계산하고 이 안에 속한 벡터를 생성하기 위해 크기 1단위로 루프를 돈다.
		{
			for (float y = -radius; y <= radius; y++)				
			{
				Vector2 pointToTest = Vector2(x, y);				//6. x,y값을 결합해 벡터(x,y)를 선언한다.
				float squaredLength = pointToTest.SizeSquared();	//7. 벡터(x,y)의 크기를 구할 때 제곱근을 씌우지 않은 x^2 + y^2값을 계산한다.
				if (squaredLength <= radius * radius)				//8. 7번값의 크기를 비교하기 위해 반지름을 2번 곱한다. 크기의 제곱값 비교와 반지름의 제곱값 비교는 제곱근을 사용하지 않기 때문에 결과가 빠르다.
				{
					circles.push_back(Vector2(x, y));				//9. 8번의 결과가 참이면 원을 구성하는 자료구조를 circles에 해당벡터를 추가한다.
				}
			}
		}
	}

	//원을 구성하는 벡터를 모두 붉은새으로 표시한다.
	for (auto const& v : circles)									//10. 원을 구성하는 자료구조 circles에 속한 모든 점을 순회한다. (C#의 foreach, 문법은 정확히 모르겠다.)
	{
		r.DrawPoint(v + currentPosition, LinearColor::Red);							//11. 각 점의 위치에 현재 중점값을 더하고 붉은색으로 찍는다.
	}

	//원의 중심좌표를 우상단에 출력
	r.PushStatisticText("Coordinate : " + currentPosition.ToString());
}

// 메시를 그리는 함수
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// 삼각형을 그리는 함수
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
