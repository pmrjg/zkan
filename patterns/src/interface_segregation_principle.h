//
// Created by pmrj on 02-01-2025.
//

#pragma once

struct Document;

struct IPrinter {
    virtual void print(Document& doc) = 0;
};

struct IScanner {
    virtual void scan(Document& doc) = 0;
};

struct IFax {
    virtual void fax(Document& doc) = 0;
};
