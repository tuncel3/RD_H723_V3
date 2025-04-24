#Persistent
#SingleInstance Force
CoordMode, Mouse, Client
SetDefaultMouseSpeed, 0

showCustomToast("ahk_livExp.ahk restart edildi.", 1000) ; 1000ms = 1sn

global clickedX := 0
global clickedY := 0
global clipboardVars := []

F4::
    clipboardVars := []

    ClipWait, 1
    clipboardText := Clipboard
    Loop, Parse, clipboardText, `n, `r
    {
        trimmed := Trim(A_LoopField)
        if (trimmed != "")
            clipboardVars.Push(trimmed)
    }

    MouseGetPos, clickedX, clickedY

    if WinExist("ahk_class #32770 ahk_exe stm32cubeide.exe") {
        WinActivate
        Sleep, 300
    }

    Loop % clipboardVars.Length()
    {
        var := clipboardVars[A_Index]
        y := clickedY + (A_Index - 1) * 18
        Click, %clickedX%, %y%, 1
        Sleep, 30
        SendInput, %var%
        SendInput, {Enter}
        Sleep, 30
    }

return

#IfWinActive ahk_exe notepad++.exe  ; F5 sadece Notepad++ aktifteyken çalışır
F5::
    SendInput, ^s
    Sleep, 300
    WinGetTitle, winTitle, A
    RegExMatch(winTitle, "^(.*\.ahk)", fullScriptPath)
    if (fullScriptPath1 != "") {
        Run, %fullScriptPath1%
        ExitApp
    } else {
        MsgBox, Script yolunu başlıktan çıkaramadım:`n%winTitle%
    }
return
#IfWinActive  ; diğer hotkey’ler etkilenmesin diye sıfırlıyoruz

showCustomToast(message := "Restart edildi.", duration := 1000) {
    Gui, ToastGui:New, -Caption +AlwaysOnTop +ToolWindow
    Gui, ToastGui:Font, s11 bold, Segoe UI
    Gui, ToastGui:Margin, 15, 10
    Gui, ToastGui:Add, Text,, %message%

    ; Sağ alt köşe
    SysGet, MonitorWorkArea, MonitorWorkArea
    WinWidth := 350
    WinHeight := 70
    x := MonitorWorkAreaRight - WinWidth - 15
    y := MonitorWorkAreaBottom - WinHeight - 15

    Gui, ToastGui:Show, x%x% y%y% w%WinWidth% h%WinHeight% NoActivate

    SetTimer, CloseCustomToast, -%duration%
}

CloseCustomToast:
    Gui, ToastGui:Destroy
return