s = \
"""
$black: #27272B
$red: #DC4850
$white: #F7FDFA
$turquoise: #43D4B4
$gray: #56595C
$gold: #FBB829

$windowWidth: 1020px
$windowHeight: 680px

AnthemUi
    background-color: $black
    min-width: $windowWidth
    max-width: $windowWidth
    min-height: $windowHeight
    max-height: $windowHeight

QLabel
    qproperty-alignment: AlignCenter
    color: $white

QPushButton
    background-color: transparent
    color: $black
    font-size: 20px
    font-family: DIN
    qproperty-flat: true
    outline: none
    border-top: 0px solid $black
    border-bottom: 2px solid $black
    border-right: 1px solid $black
    border-left: 1px solid $black
    min-width: 40px
    &:open
        background-color: $black
        color: $red

Menubar
    $menuHeight: 60px
    $borderWidth: 3px
    $fitted: $menuHeight - 2 * $borderWidth
    min-height: $menuHeight + 20
    max-height: $menuHeight + 20
    & #CopyDock
        border: $borderWidth solid $red
        min-width: $fitted
        min-height: $fitted
        max-width: $fitted
        max-height: $fitted
        background-color: $black
        font-size: 30px
        font-family: DIN
        color: $red
        &:enabled
            background-color: $red
            color: $black
    & IconButton
        border: $borderWidth solid $red
        min-width: $fitted
        min-height: $fitted
        max-width: $fitted
        max-height: $fitted
        qproperty-iconWidth: 35
        qproperty-iconHeight: 35
    & #VolumeLabel
        min-width: $menuHeight
        min-height: $menuHeight
        max-width: $menuHeight
        max-height: $menuHeight
    Projectbar
        background-color: $red
        min-width: 500px
        max-width: 500px

#ProjectLabel
    color: $black
    font-family: DIN
    font-size: 30px
    min-width: 350px
    max-width: 350px
    padding-right: 15px

#PreviousButton, #NextButton
    qproperty-iconWidth: 25
    qproperty-iconHeight: 25
    min-width: 35px
    max-width: 35px

#PreviousButton
    padding-right: 5px

#MenuButton
    qproperty-iconWidth: 30
    qproperty-iconHeight: 30
    min-width: 35px
    max-width: 35px
    min-height: 35px
    max-height: 35px

QMenu
    background-color: $red
    margin: 15px
    &::item
        color: $black
        padding: 15px
        font-size: 16px
        &::selected
            background-color: $black
            color: $red
        &::checked
            background-color: $black
            color: $red
            padding-left: 30px

CustomComboBox QMenu
    margin: 0px
    &::item
        padding: 40px

QDialog
    background-color: $red
    & QPushButton
        background-color: transparent
        color: $black
        font-size: 18px
        font-family: "Proxima Nova"
        qproperty-flat: true
        outline: none
        border: 1px solid $black
        min-height: 50px
        &:hover
            background-color: $black
            color: $red

SettingsDialog
    @extend QDialog
    & QLabel
        color: $black
        font-family: "Proxima Nova"
        font-size: 20px

CustomMessageBox
    & QLabel
        color: $black
        font-family: "Proxima Nova"
        font-size: 30px

PopupLine QPushButton
    border: none
    &:hover
        background: none

IconButton
    background-color: none
    outline: none
    border: none

QLineEdit
    font-size: 30px
    border: none
    qproperty-frame: false
    background-color: $red
    color: $black

QComboBox
    background-color: transparent
    color: $black
    font-size: 20px
    font-family: "Proxima Nova"
    border: 1px solid $black
    min-height: 50px
    padding-left: 34px
    &::drop-down
        border: none
    &:hover
        background-color: $black
        color: $red
    & QListView
        background: $red
        color: $black

QTabWidget
    min-width: $windowWidth
    &::pane
        background-color: transparent
        margin: 0px
        padding: 0px
        max-height: 0px
        max-width: 0px
        border-width: 0px
    & QTabBar::tab
        background-color: $black
        font-size: 30px
        color: $white
        font-family: "DIN Light"
        border: none
        outline: none
        min-width: $windowWidth / 5
        max-width: $windowWidth / 5
        min-height: 50px
        max-height: 50px
        padding-bottom: 0px
        padding-top: 5px
        margin: 0px
        text-align: center
        qproperty-alignment: AlignCenter
        &:selected, &:hover
            background-color: $red
            color: $black

OperatorPage
    background-color: $red
    & QTabBar::tab
        min-width: 250px
        min-height: 40px
        padding-top: 10px
        font-size: 30px
        margin: 0px

OperatorUi, PanUi
    border: 2px solid $black

ModDial
    background-color: transparent
    font-size: 14px
    font-family: DIN
    color: $black
    qproperty-arcColor: $black
    qproperty-arcWidth: 3
    qproperty-arcPadding: 4
    qproperty-valueShown: false
    qproperty-displayedArcColor: $white
    min-width: 120
    min-height: 120

ModItemUi
    background-color: $red
    qproperty-borderColor: $black
    qproperty-borderWidth: 4
    color: $black
    font-family: DIN
    font-size: 14px
    min-width: 35px
    min-height: 35px
    & QMenu::item
        padding-right: 30px

AlgorithmUi
    background-color: transparent
    border: 2px solid $black
    outline: none
    & QLabel
        background-color: $red
        color: $black
        font-size: 20px
        font-family: DIN
        border: 2px solid $black
        min-width: 24px
        max-width: 24px
        min-height: 30px
        max-height: 30px
        &:enabled
            background-color: $black
            color: $red

MasterPage
    background-color: $red
    & ModDial
        font-size: 15px

#VolumeUi
    border: 2px solid $black
    padding-top: 50px
"""

if __name__ == "__main__":

	l = s.split("\n\n")

	l.sort(key=lambda x: x.split()[0][0] if x.split()[0][0].isalpha() else x.split()[0][1])

	print("\n\n".join(l))



