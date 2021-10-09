//GPU���� ����� Constant Buffer(cbuffer) ����ü ����
//CPU���� ����ϴ� Constant Buffer ����ü�� ������ ũ�� �� ��ġ�� �����ؾ� 
//��Ȯ�� ��� ���� ��µ�
//hlsl�� �⺻������ �����͸� �� �켱 ������� ä��µ�
//����ü ��� �տ� row_major Ű���带 ���̸� �ش� ����� �� �켱 ������� �����͸� ä��
//���� DirectX -> Shader ���α׷����� �����͸� ���� �� ����� 2������
//1. DirectX���� ����� ��ġ���� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => ����ġ => DirectX�� ��� ������ ���� �״�� ����
//2. DirectX���� ����� �״�� Shader ���α׷����� ������ �� �켱 ������� �����͸� ä�� => DirectX�� ��� ������ ���� �״�� ����
cbuffer VS_WVPMatrix : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
}
