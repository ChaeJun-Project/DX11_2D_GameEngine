del /q /s ".\External\Include\DX11_2D_GameEngine_Lib"
xcopy /s /y  /exclude:External_Exclude_List.txt ".\Project\DX11_2D_GameEngine_Lib\*.h" ".\External\Include\DX11_2D_GameEngine_Lib"
xcopy /s /y  /exclude:External_Exclude_List.txt ".\Project\DX11_2D_GameEngine_Lib\Math\*.h" ".\External\Include\DX11_2D_GameEngine_Lib\Math"
xcopy /s /y  /exclude:External_Exclude_List.txt ".\Project\DX11_2D_GameEngine_Lib\ShaderFiles\*.fx" ".\ExeFile\Content\Shader"