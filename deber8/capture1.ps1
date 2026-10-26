Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

# Maximizar la ventana actual
$wshell = New-Object -ComObject WScript.Shell
$wshell.SendKeys('%{ENTER}')   # Alt+Enter -> fullscreen console (toggle)
Start-Sleep -Milliseconds 500

# Correr el exe
Write-Host ""
& "C:\Users\User\Desktop\8vo\Algorithms\lab8\activity1\swap.exe"
Write-Host ""
Write-Host "----------------------------------------------"
Write-Host "  Screenshot en 3 segundos..."
Write-Host "----------------------------------------------"

Start-Sleep -Seconds 3

# Capturar pantalla completa
$screen = [System.Windows.Forms.Screen]::PrimaryScreen.Bounds
$bmp = New-Object System.Drawing.Bitmap($screen.Width, $screen.Height)
$g   = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($screen.Location, [System.Drawing.Point]::Empty, $screen.Size)
$bmp.Save("C:\Users\User\Desktop\8vo\Algorithms\lab8\activity1\output.png",
          [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
Write-Host "Guardado: activity1\output.png"
Start-Sleep -Seconds 1
