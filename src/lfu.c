#include <limits.h>
#include <stdio.h>
#define MAX_FRAMES 10
#define MAX_PAGES 100
typedef struct {
  int page;
  int frequency;
} Frame;
void initialize_frames(Frame frames[], int n) {
  for (int i = 0; i < n; i++) {
    frames[i].page = -1;
    frames[i].frequency = 0;
  }
}
int find_page(Frame frames[], int n, int page) {
  for (int i = 0; i < n; i++) {
    if (frames[i].page == page) {
      return i;
    }
  }
  return -1;
}
int find_lfu_frame(Frame frames[], int n) {
  int min_freq = INT_MAX;
  int lfu_index = -1;
  for (int i = 0; i < n; i++) {
    if (frames[i].frequency < min_freq) {
      min_freq = frames[i].frequency;
      lfu_index = i;
    }
  }
  return lfu_index;
}
void print_frames(Frame frames[], int n) {
  for (int i = 0; i < n; i++) {
    if (frames[i].page != -1) {
      printf("%d (freq: %d) ", frames[i].page, frames[i].frequency);
    } else {
      printf("- ");
    }
  }
  printf("\n");
}
void lfu_page_replacement(const int pages[], int num_pages, int num_frames) {
  Frame frames[MAX_FRAMES];
  initialize_frames(frames, num_frames);
  int page_faults = 0;
  for (int i = 0; i < num_pages; i++) {
    int page = pages[i];
    int page_index = find_page(frames, num_frames, page);
    if (page_index != -1) {
      frames[page_index].frequency++;
    } else {
      int lfu_index = find_lfu_frame(frames, num_frames);
      frames[lfu_index].page = page;
      frames[lfu_index].frequency = 1;
      page_faults++;
    }
    print_frames(frames, num_frames);
  }
  printf("Total page faults: %d\n", page_faults);
}
int main() {
  int num_frames, num_pages; //NOLINT
  printf("Enter number of frames: ");
  scanf("%d", &num_frames);
  printf("Enter number of pages: ");
  scanf("%d", &num_pages);
  int pages[MAX_PAGES];
  printf("Enter the page sequence:\n");
  for (int i = 0; i < num_pages; i++) {
    scanf("%d", &pages[i]);
  }
  lfu_page_replacement(pages, num_pages, num_frames);
  return 0;
}
