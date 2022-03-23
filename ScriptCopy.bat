del /q /s ".\External\Include\Script_Lib"
xcopy /s /y  /exclude:External_Exclude_List.txt ".\Project\Script_Lib\*.h" ".\External\Include\Script_Lib"
