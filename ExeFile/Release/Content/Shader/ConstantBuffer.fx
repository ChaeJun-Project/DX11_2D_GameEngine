//GPU���� ����� Constant Buffer(cbuffer) ����ü ����
//CPU���� ����ϴ� Constant Buffer ����ü�� ������ ũ�� �� ��ġ�� �����ؾ� 
//��Ȯ�� ��� ���� ��µ�
cbuffer VS_WVPMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
}
