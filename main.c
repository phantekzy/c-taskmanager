#include <dirent.h>
#include <stdio.h>
// UI
#define BORDER "\033[38;5;244m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Global snapshots
long long last_total = 0, last_idle = 0;
// CPU
void get_cpu_usage(double *cpu_perc) {
  long long a, b, c, idle, e, f, g;
  FILE *fp = fopen("/proc/stat", "r");
  fscanf(fp, "cpu %lld %lld %lld %lld %lld %lld %lld", &a, &b, &c, &idle, &e,
         &f, &g);
  fclose(fp);
  long long total = a + b + c + idle + e + f + g;
  long long diff_total = total - last_total;
  long long diff_idle = idle - last_idle;
  // CPU PERCENTAGE
  *cpu_perc =
      (diff_total > 0) ? (100 * (diff_total - diff_idle) / diff_total) : 0;
  last_total = total;
  last_idle = idle;
}

// RAM
void get_mem_usage(double *usage, double *total_gb) {
  long total, available;
  FILE *fp = fopen("/proc/meminfo", "r");
  fscanf(fp, "MemTotal: %ld kB\nMemFree: %ld kB\nMemAvailable: %ld kB", &total,
         &available, &available);
  fclose(fp);
  *total_gb = (double)total / 1024 / 1024;
  *usage = ((*total_gb) - (double)available / 1024 / 1024);
}
// DRAW BAR
void draw_bar(double percent, char *color) {
  int width = 50;
  int filled = (int)(percent * width / 100);
  printf("[");
  printf("%s", color);
  for (int i = 0; i < width; i++) {
    if (i < filled)
      printf("■");
    else
      printf(" ");
  }
  printf(RESET "] %.1f%%\n", percent);
}

int main() { return 0; }
