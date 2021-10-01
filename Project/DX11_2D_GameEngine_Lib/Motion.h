#pragma once

class Mesh;
class Texture;

class Motion
{
public: 
   Motion();
   ~Motion();


private:
   std::vector<Texture> m_texture_vector;

};

