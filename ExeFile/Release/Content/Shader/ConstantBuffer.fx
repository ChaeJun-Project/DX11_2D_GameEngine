//GPU에서 사용할 Constant Buffer(cbuffer) 구조체 정의
//CPU에서 사용하는 Constant Buffer 구조체와 변수의 크기 및 위치가 동일해야 
//정확한 결과 값이 출력됨
cbuffer VS_WVPMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}
