#Persistent
#SingleInstance Force
CoordMode, Mouse, Client  ; Koordinatlar sadece pencere içi geçerli
SetDefaultMouseSpeed, 0

global clickedX := 0
global clickedY := 0
global targetWindow := ""
;global defaultVars := ["blm_balance_accepted", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge", "blm_current_detected_cnt", "blm_req_monitor_balance", "blm_balance_accept_cnt", blm_req_reduce_vtarge"]



StartGUI() {
    global

    Gui, New, +LabelMainGui
    Gui, Add, Button, gMainGuiKoordinatAl x10 y10, Hücreye Tıklayıp Değişkenleri Yaz
    Gui, Add, Text, vKoordinatText x+10 yp w400, Henüz tıklanmadı

    Gui, Add, Button, gMainGuiPasteClipboard x10 y+10, 📋 Notepad'den Yapıştır
    Gui, Add, Button, gMainGuiListeyiTemizle x+10 yp, 🧹 Listeyi Temizle
    Gui, Add, Button, gMainGuiRestartGui x+10 yp, 🔄 Yeniden Başlat

    Gui, Add, ListView, vDegiskenListesi x10 y+10 w600 h500, Variable Name

    ; 🔽 DEFAULT LİSTEYİ YÜKLE
    ;for each, var in defaultVars
    ;    LV_Add("", var)

    Gui, Show,, Live Expression Otomat
}
StartGUI()
return

MainGuiKoordinatAl:
    GuiControl,, KoordinatText, İlk tıklama (pencere seçimi)...
    KeyWait, LButton, D
    Sleep, 300

    GuiControl,, KoordinatText, Şimdi ilk hücreye tıklayın...
    KeyWait, LButton, D
    MouseGetPos, clickedX, clickedY
    GuiControl,, KoordinatText, Başlangıç X: %clickedX% Y: %clickedY%

    ; Pencereyi öne al
    if WinExist("ahk_class #32770 ahk_exe stm32cubeide.exe") {
        WinActivate
        Sleep, 300
    }

    ; Listede kaç değişken varsa sırayla işle
    RowCount := LV_GetCount()
    iptalIstendi := false  ; her başta sıfırla
    Loop, %RowCount%
    {
        if (iptalIstendi)
            break

        index := A_Index
        LV_GetText(var, index, 1)
        if (var = "")
            continue

        y := clickedY + (index - 1) * 18
        Click, %clickedX%, %y%, 1
        Sleep, 20
        SendInput, %var%
        SendInput, {Enter}
        Sleep, 20
    }


return

MainGuiPasteClipboard:
    ClipWait, 1
    clipboardText := Clipboard
    Loop, Parse, clipboardText, `n, `r
    {
        trimmed := Trim(A_LoopField)
        if (trimmed != "")
            LV_Add("", trimmed)
    }
return

MainGuiListeyiTemizle:
    LV_Delete()
return

MainGuiRestartGui:
    Run, %A_ScriptFullPath%
    ExitApp
return


MainGuiClose:
ExitApp

F12::
    iptalIstendi := true
return
