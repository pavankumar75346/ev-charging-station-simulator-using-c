#include <stdio.h>
#include <windows.h>
#include <time.h>

#define RATE 10

struct Charger {
    int id;
    int power_kw;
    int busy;   // 0 = free, 1 = occupied
};

struct Charger chargers[3] = { {1, 7, 0}, {2, 22, 0}, {3, 50, 0} };

void show_chargers() {
    for (int i = 0; i < 3; i++) {
        printf("Charger %d (%d kW): %s\n",
               chargers[i].id, chargers[i].power_kw,
               chargers[i].busy ? "Occupied" : "Available");
    }
}

void save_session(int charger_id, float energy, float cost) {
    FILE *chk = fopen("sessions.csv", "r");
    int exists = (chk != NULL);
    if (chk) fclose(chk);

    FILE *f = fopen("sessions.csv", "a");
    if (f == NULL) return;
    if (!exists) fprintf(f, "time,vehicle,charger,energy_kwh,cost\n");

    time_t now = time(NULL);
    char t[30];
    strftime(t, sizeof(t), "%Y-%m-%d %H:%M", localtime(&now));
    fprintf(f, "%s,EV,%d,%.2f,%.2f\n", t, charger_id, energy, cost);
    fclose(f);
}

void charge(struct Charger *c, float capacity, float battery) {
    float start = battery;
    c->busy = 1;
    while (battery < 100) {
        float kwh = c->power_kw / 60.0;
        battery += kwh / capacity * 100;
        if (battery > 100) battery = 100;
        printf("Battery: %.1f%%\n", battery);
        Sleep(200);   // wait 0.2 seconds
    }
    c->busy = 0;
    float used = (battery - start) / 100 * capacity;
    printf("Done. Energy: %.2f kWh, Cost: %.2f\n", used, used * RATE);
    save_session(c->id, used, used * RATE);
}

int main() {
    int choice;
    while (1) {
        printf("\n1. Show chargers\n2. Start charging\n3. View history\n4. Exit\nChoose: ");
        scanf("%d", &choice);

        if (choice == 1) {
            show_chargers();
        } else if (choice == 2) {
            int id;
            float battery;
            show_chargers();
            printf("Charger number: ");
            scanf("%d", &id);
            if (id < 1 || id > 3 || chargers[id - 1].busy) {
                printf("Not available.\n");
                continue;
            }
            printf("Current battery %%: ");
            scanf("%f", &battery);
            charge(&chargers[id - 1], 40, battery);
        } else if (choice == 3) {
            FILE *f = fopen("sessions.csv", "r");
            if (f == NULL) {
                printf("No sessions yet.\n");
            } else {
                char line[200];
                while (fgets(line, sizeof(line), f)) printf("%s", line);
                fclose(f);
            }
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}