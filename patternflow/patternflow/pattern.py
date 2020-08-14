# -*- coding: utf-8 -*-


class Pattern:
    def __init__(self, page=8, row=8, column=8):
        self.page = page
        self.row = row
        self.column = column
        self.init_data()

    def init_data(self):
        self.data = []
        self.page_flag = []
        self.row_flag = []
        self.column_flag = []
        for i in range(0, self.page):
            page_data = []
            self.page_flag.append(0)
            a_row_flag = []
            for i in range(0, self.row):
                a_row_flag.append(0)
                a_column_flag = []
                for j in range(0, self.column):
                    a_column_flag.append(0)
                    page_data.append(0)
            self.column_flag.append(a_column_flag)
            self.row_flag.append(a_row_flag)
            self.data.append(page_data)

    def __str__(self):
        return self.format(format='hex')

    def __repr__(self):
        return '<Pattern Object %dx%dx%d>' % (self.page, self.row, self.column)

    def set(self, page, row, column, value):
        self.data[page][row * 8 + column] = value
        for bit in self.data[page]:
            self.page_flag[page] = bit
            if bit == 0:
                break
        start = self.column * row
        end = start + self.column
        for bit in self.data[page][start:end]:
            self.row_flag[page][row] = bit
            if bit == 0:
                break

        start = column
        step = self.column
        end = start + step * self.row
        for bit in self.data[page][start:end:step]:
            self.column_flag[page][column] = bit
            if bit == 0:
                break

    def set_row(self, page, row, value):
        for i in range(0, self.column):
            self.set(page, row, i, value)

    def set_column(self, page, column, value):
        for i in range(self.row):
            self.set(page, i, column, value)

    def set_page(self, page, value):
        for i in range(self.row * self.column):
            self.data[page][i] = value
        self.page_flag[page] = value
        for i in range(self.row):
            self.row_flag[page][i] = value
        for i in range(self.column):
            self.column_flag[page][i] = value

    def set_all(self, value):
        for i in range(0, self.page):
            self.set_page(i, value)

    def zip(self):
        zipped_data = []
        for row in range(self.row):
            for page in range(self.page):
                column_sum = 0
                for column in range(self.column):
                    bit = self.data[page][row * self.column + column]
                    if bit:
                        column_sum += 2**column
                zipped_data.append(column_sum)
        return zipped_data

    def unzip(self, zipped_data):
        unzipped_data = []
        zipped_data_t = []
        for p in range(self.page):
            p_data_t = []
            for pt in zipped_data:
                p_data_t.append(pt[p])
            zipped_data_t.append(p_data_t)
        for p in zipped_data_t:
            page = []
            for r in p:
                c = list(bin(r)[2:].zfill(8))
                cint = []
                for i in c[::-1]:
                    cint.append(int(i))
                page.extend(cint)
            unzipped_data.append(page)
        return unzipped_data

    def format(self, format='hex', sep=', ', beautify=True):
        data = self.zip()
        data_str = []
        if format == 'dec':
            for i in data:
                data_str.append(str(i))
        elif format == 'hex':
            for i in data:
                data_str.append('0x%.2X' % i)
        else:
            raise ValueError("No such format '%s'" % format)

        if beautify:
            data_beauty = []
            for i in range(0, self.row):
                start = self.page * i
                end = start + 8
                a_row = sep.join(data_str[start:end])
                data_beauty.append(a_row)
            data_beauty_str = ',\n'.join(data_beauty)
            return data_beauty_str
        else:
            return sep.join(data_str)

    def restore(self, text):
        text = text.split(',\n')
        self.page = len(text)
        zipped_data = []
        for p in text:
            rows = p.split(', ')
            self.row = len(rows)
            rows = [int(i, 0) for i in rows]
            zipped_data.append(rows)
        unzipped_data = self.unzip(zipped_data)
        self.init_data()
        for page in range(self.page):
            for row in range(self.row):
                for column in range(self.column):
                    index = self.column * row + column
                    new_bit = unzipped_data[page][index]
                    if new_bit:
                        self.set(page, row, column, 1)
                    else:
                        self.set(page, row, column, 0)


if __name__ == '__main__':
    p = Pattern(8, 8, 8)
    # print(p.page_flag)
    # print(p.row_flag)
    # print(p.column_flag)
    # for i in range(8):
    #     p.set_column(0, i, 1)
    #     p.set(0, i, 0, 0)
    # print(p.format('dec'))
    # print(p.page_flag)
    # print(p.row_flag)
    # print(p.column_flag)
    hex_text = """0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00"""
    # p.restore(dec_text)
    p.restore(hex_text)
    print(p.data)
    print(p.page_flag)
    print(p.row_flag)
    print(p.column_flag)
