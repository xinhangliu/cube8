# -*- coding: utf-8 -*-
import sys
from PyQt5.QtWidgets import (
    QMainWindow, QDesktopWidget, QApplication, QWidget,
    QVBoxLayout, QHBoxLayout, QGridLayout,
    QPushButton, QComboBox, QTextEdit, QCheckBox
)
from PyQt5.QtCore import QCoreApplication, Qt
from PyQt5.QtGui import QGuiApplication
from pattern import Pattern


class PatternFlow(QWidget):
    def __init__(self, page=8, row=8, column=8):
        super().__init__()
        self.page = page
        self.row = row
        self.column = column

        self.pt = Pattern()
        self.format = 'dec'

        self.cb = QGuiApplication.clipboard()

        matrixs = self.matrixs(8)

        console = self.console()

        main_layout = QVBoxLayout()
        main_layout.addLayout(matrixs)
        main_layout.addStretch(1)
        main_layout.addLayout(console)

        self.setLayout(main_layout)
        self.center_windows()
        self.setWindowTitle('PatternFlow')
        self.resize(800, 400)
        self.show()

    def console(self):
        format_cbo = QComboBox()
        format_cbo.addItem('Dec')
        format_cbo.addItem('Hex')
        format_cbo.activated[str].connect(self.set_format)

        self.code_text = QTextEdit()
        copy_btn = QPushButton('Copy')
        copy_btn.setToolTip('Copy the pattern code to the clipboard')
        copy_btn.clicked.connect(self.copy_to_clipboard)

        reset_btn = QPushButton('Reset')
        reset_btn.setToolTip('Reset all bit to 0')
        reset_btn.clicked.connect(self.reset)
        setall_btn = QPushButton('Set All')
        setall_btn.setToolTip('Set all bit to 1')
        setall_btn.clicked.connect(self.setall)

        restore_btn = QPushButton('Restore')
        restore_btn.clicked.connect(self.restore)
        restore_btn.setToolTip('Restore pattern from code')

        quit_btn = QPushButton('Quit')
        quit_btn.clicked.connect(QCoreApplication.instance().quit)

        console = QGridLayout()
        console.addWidget(self.code_text, 0, 0, 6, 4)
        console.setColumnStretch(0, 1)
        console.addWidget(format_cbo, 0, 6)
        console.addWidget(copy_btn, 1, 6)
        console.addWidget(reset_btn, 3, 6)
        console.addWidget(setall_btn, 2, 6)
        console.addWidget(restore_btn, 4, 6)
        console.addWidget(quit_btn, 5, 6)
        return console

    def reset(self):
        self.pt.init_data()
        self.mapto_btn()
        self.code_text.clear()

    def setall(self):
        for i in range(self.page):
            self.pt.set_page(i, 1)
        self.mapto_btn()
        self.gen_result()

    def copy_to_clipboard(self):
        text = self.code_text.toPlainText()
        self.cb.setText(text)

    def matrixs(self, page):
        self.bit_btns = []
        matrixs = QGridLayout()
        for page in range(8):
            matrix = QGridLayout()
            matrix.setSpacing(2)
            set_page = QPushButton()
            set_page.setFixedSize(16, 16)
            set_page.setToolTip('set_page(%d' % page)
            set_page.setStyleSheet(
                'QPushButton {background-color: #fff; border: 1px solid #ccc; border-radius: 4px}'
                'QPushButton:open {background-color: #666; border: 1px solid #ccc}'
            )
            set_page.clicked.connect(self.set_page)
            matrix.addWidget(set_page, 0, 0)
            for row in range(8):
                set_row = QPushButton()
                set_row.setFixedSize(16, 16)
                set_row.setToolTip('set_row(%d, %d)' % (page, row))
                set_row.setStyleSheet(
                    'QPushButton {background-color: #fff; border: 1px solid #ccc; border-radius: 4px}'
                    'QPushButton:open {background-color: #aaa}'
                )
                set_row.clicked.connect(self.set_row)
                matrix.addWidget(set_row, row + 1, 0)
            for column in range(8):
                set_column = QPushButton()
                set_column.setFixedSize(16, 16)
                set_column.setToolTip('set_column(%d, %d)' % (page, column))
                set_column.setStyleSheet(
                    'QPushButton {background-color: #fff; border: 1px solid #ccc; border-radius: 4px}'
                    'QPushButton:open {background-color: #aaa}'
                )
                set_column.clicked.connect(self.set_column)
                matrix.addWidget(set_column, 0, column + 1)
            btn_page = []
            for row in range(8):
                for column in range(8):
                    name = 'set_bit(%d, %d, %d)' % (page, row, column)
                    bit_btn = QPushButton()
                    bit_btn.setToolTip(name)
                    bit_btn.setFixedSize(16, 16)
                    bit_btn.setCheckable(True)
                    bit_btn.setStyleSheet(
                        'QPushButton {background: #ddd; border: none; margin: 0px; padding: 0px}'
                        'QPushButton:checked {background-color: #2c9e4b; border: none}'
                    )
                    bit_btn.clicked[bool].connect(self.set_bit)
                    btn_page.append(bit_btn)
                    matrix.addWidget(bit_btn, row + 1, column + 1)
            self.bit_btns.append(btn_page)
            matrixs.addLayout(matrix, page / 4, page % 4)
        return matrixs

    def mapto_code(self):
        for page in range(self.page):
            for row in range(self.row):
                for column in range(self.column):
                    bit_btn = self.bit_btns[page][self.column * row + column]
                    if bit_btn.isChecked():
                        self.pt.set(page, row, column, 1)
                    else:
                        self.pt.set(page, row, column, 0)

    def mapto_btn(self):
        for page in range(self.page):
            for row in range(self.row):
                for column in range(self.column):
                    bit_btn = self.bit_btns[page][self.column * row + column]
                    bit = self.pt.data[page][self.column * row + column]
                    if bit == 1:
                        bit_btn.setChecked(True)
                    else:
                        bit_btn.setChecked(False)

    def restore(self):
        text = self.code_text.toPlainText()
        if text:
            try:
                self.pt.restore(text)
                self.mapto_btn()
            except (ValueError, IndexError):
                pass

    def set_format(self, text):
        self.format = text.lower()
        self.gen_result()

    def gen_result(self):
        self.code_text.setText(self.pt.format(self.format))

    def set_page(self, state):
        sender = self.sender()
        page = sender.toolTip().strip('set_page(').strip(')')
        page = int(page)
        if self.pt.page_flag[page] == 0:
            self.pt.set_page(page, 1)
            for bit_btn in self.bit_btns[page]:
                bit_btn.setChecked(True)
        else:
            self.pt.set_page(page, 0)
            for bit_btn in self.bit_btns[page]:
                bit_btn.setChecked(False)
        self.gen_result()

    def set_row(self, state):
        sender = self.sender()
        text = sender.toolTip().strip('set_row(').strip(')').split(', ')
        page = int(text[0])
        row = int(text[1])
        start = self.column * row
        end = start + self.column
        if self.pt.row_flag[page][row]:
            self.pt.set_row(page, row, 0)
            for bit_btn in self.bit_btns[page][start:end]:
                bit_btn.setChecked(False)
        else:
            self.pt.set_row(page, row, 1)
            for bit_btn in self.bit_btns[page][start:end]:
                bit_btn.setChecked(True)
        self.gen_result()

    def set_column(self, state):
        sender = self.sender()
        text = sender.toolTip().strip('set_column(').strip(')').split(', ')
        page = int(text[0])
        column = int(text[1])
        start = column
        step = self.column
        end = start + step * self.row
        if self.pt.column_flag[page][column]:
            self.pt.set_column(page, column, 0)
            for bit_btn in self.bit_btns[page][start:end:step]:
                bit_btn.setChecked(False)
        else:
            self.pt.set_column(page, column, 1)
            for bit_btn in self.bit_btns[page][start:end:step]:
                bit_btn.setChecked(True)
        self.gen_result()

    def set_bit(self, pressed):
        sender = self.sender()
        text = sender.toolTip().strip('set_bit(').strip(')').split(', ')
        page = int(text[0])
        row = int(text[1])
        column = int(text[2])
        if pressed:
            self.pt.set(page, row, column, 1)
        else:
            self.pt.set(page, row, column, 0)
        self.gen_result()

    def center_windows(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())


if __name__ == '__main__':
    app = QApplication(sys.argv)
    pf = PatternFlow()
    sys.exit(app.exec_())
