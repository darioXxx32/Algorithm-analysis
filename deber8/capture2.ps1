Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
$wshell = New-Object -ComObject WScript.Shell
$wshell.SendKeys('%{ENTER}')
Start-Sleep -Milliseconds 500

Write-Host ""
& "C:\Users\User\Desktop\8vo\Algorithms\lab8\activity2\ccs.exe"
Write-Host ""
Write-Host "----------------------------------------------"
Write-Host "  Screenshot en 3 segundos..."
Write-Host "----------------------------------------------"

Start-Sleep -Seconds 3

$screen = [System.Windows.Forms.Screen]::PrimaryScreen.Bounds
$bmp = New-Object System.Drawing.Bitmap($screen.Width, $screen.Height)
$g   = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($screen.Location, [System.Drawing.Point]::Empty, $screen.Size)
$bmp.Save("C:\Users\User\Desktop\8vo\Algorithms\lab8\activity2\output.png",
          [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
Write-Host "Guardado: activity2\output.png"
Start-Sleep -Seconds 1
