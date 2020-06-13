import numpy as np
import datetime
import msvcrt
import zeep
from onvif import ONVIFCamera


def zeep_pythonvalue(self, xmlvalue):
    return xmlvalue
zeep.xsd.simple.AnySimpleType.pythonvalue = zeep_pythonvalue


if __name__ == '__main__':
    mycam = ONVIFCamera('172.18.212.17', 80, 'laba2102', 'TMPpassword', 'C:/wsdl/')
    media = mycam.create_media_service()
    media_profile = media.GetProfiles()[0]
    ptz = mycam.create_ptz_service()

    abs_move = ptz.create_type('AbsoluteMove')
    abs_move.ProfileToken = media_profile.token

    imaging = mycam.create_imaging_service()
    brightness = imaging.create_type('SetImagingSettings')
    brightness.VideoSourceToken = media_profile.VideoSourceConfiguration.SourceToken
    brightness.ImagingSettings = imaging.GetImagingSettings({
        'VideoSourceToken': media_profile.VideoSourceConfiguration.SourceToken
    })
    cur_bright = int(brightness.ImagingSettings.Brightness)

    coords = np.linspace(-1, 1, 10)
    coords = {str(i): coords[i] for i in range(0, 10)}

    while True:
        ch = msvcrt.getch()
        ch = ch.lower().decode('utf-8')

        if ch in [str(i) for i in range(0, 10)]:
            abs_move.Position = ptz.GetStatus({'ProfileToken': media_profile.token}).Position
            abs_move.Position.PanTilt.x = coords[ch]
            ptz.AbsoluteMove(abs_move)
            print('Новые координаты камеры:', abs_move.Position.PanTilt.x, abs_move.Position.PanTilt.y)

        elif ch == '[':
            cur_bright -= 10
            if cur_bright < 0: cur_bright = 0
            brightness.ImagingSettings.Brightness = cur_bright
            imaging.SetImagingSettings(brightness)
            print('Установленная яркость', cur_bright)

        elif ch == ']':
            cur_bright += 10
            if cur_bright > 100: cur_bright = 100
            brightness.ImagingSettings.Brightness = cur_bright
            imaging.SetImagingSettings(brightness)
            print('Установленная яркость', cur_bright)

        elif ch == 'p':
            abs_move.Position = ptz.GetStatus({'ProfileToken': media_profile.token}).Position
            print('Координаты камеры:', abs_move.Position.PanTilt.x, abs_move.Position.PanTilt.y)

        elif ch == 'q':
            exit()
        # else:
        #     print(ch)
