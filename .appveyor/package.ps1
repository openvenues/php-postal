$ErrorActionPreference = "Stop"

Set-Location C:\projects\php-postal

$dname = ''
if ($env:ARCH -eq 'x64') {
    $dname += 'x64\'
}
$dname += 'Release';
if ($env:TS -eq '1') {
    $dname += '_TS'
}

$files = 
    'C:\projects\php-postal\README.md',
    'C:\projects\php-postal\LICENSE',
    'C:\build-cache\deps\bin\libpostal.dll',
    "$dname\php_postal.dll"

Compress-Archive $files 'php-postal.zip' -CompressionLevel 'Optimal'
