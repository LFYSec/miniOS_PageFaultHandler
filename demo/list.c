#include <stdio.h>
#include "../include/list.h"

// 需要被链表串起来的结构体中需要有link_head结构体
struct page {
    char name[16];
    struct link_head node;
};

int main(int argc, const char *argv[])
{
    // 链表头
    struct link_head pages;

    // 链表项
    struct page page1 = {"1"};
    struct page page2 = {"2"};
    struct page page3 = {"3"};

    // 初始化链表头
    link_init(&pages);
    // append，向链表后加入
    link_append(&pages, &page1.node);
    link_append(&pages, &page2.node);
    // prepend，向链表前加入
    link_prepend(&pages, &page3.node);

    printf("entries: ");
    // 遍历链表，第二个参数为迭代变量
    link_foreach(&pages, p) {
        // link_entry可以通过link_head结构体来找到实际的数据结构体
        // 第一个参数为link_head的指针，第二个参数为数据结构体的名称，
        // 第三个参数为link_head结构体在数据结构体中的字段名。
        printf("%s ", link_entry(p, struct page, node)->name);
    }
    printf("\n");

    int len = 0;
    len = link_length(&pages);
    printf("length: %d\n", len);

    printf("remove p1: %x %x\n", link_remove(&pages, &page1.node), &page1.node);
    printf("entries: ");
    link_foreach(&pages, p) {
        printf("%s ", link_entry(p, struct page, node)->name);
    }
    printf("\n");

    len = link_length(&pages);
    printf("length: %d\n", len);

    printf("remove p2: %x %x\n", link_remove(&pages, &page2.node), &page2.node);
    printf("remove p3: %x %x\n", link_remove(&pages, &page3.node), &page3.node);
    printf("entries: ");
    link_foreach(&pages, p) {
        printf("%s ", link_entry(p, struct page, node)->name);
    }
    printf("\n");

    printf("trying to remove empty: %x %x\n", link_remove(&pages, &page3.node), &page3.node);

    len = link_length(&pages);
    printf("length: %d\n", len);

    link_append(&pages, &page1.node);
    link_append(&pages, &page2.node);
    link_append(&pages, &page3.node);
    printf("entries: ");
    link_foreach(&pages, p) {
        printf("%s ", link_entry(p, struct page, node)->name);
    }
    printf("\n");

    return 0;
}
