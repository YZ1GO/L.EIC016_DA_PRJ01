#include "Menu.h"

using namespace std;

string reservoirCSV = "../small-dataSet/Reservoirs_Madeira.csv";
string stationsCSV = "../small-dataSet/Stations_Madeira.csv";
string citiesCSV = "../small-dataSet/Cities_Madeira.csv";
string pipesCSV = "../small-dataSet/Pipes_Madeira.csv";

Menu::Menu() : parser(reservoirCSV, stationsCSV, citiesCSV, pipesCSV), bsm(parser.getCodeGraph(), parser.getDataContainer()){
    menuIndex = {
            makeBold("[1] Print All Data Container"),
            makeBold("[2] Print Specific Data Container"),
            makeBold("[3] Total Maximum Flow"),
            makeBold("[4] Cities With Water Deficit"),
            makeBold("[5] Remove Reservoir"),
            makeBold("[9] ⚠\uFE0F RESET GRAPH ⚠\uFE0F"),
            makeBold("[0] EXIT")
    };
}

void Menu::clearScreen() {
    system("clear || cls");
}

void Menu::waitPress() {
    cout << "\n";
    cin.get();
    cout << "Press ENTER to continue...";
    cin.get();
}

int Menu::inputParser(int& choice, string text) {
    cout << "\n" + text;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 1;
    }
    return 0;
}

void Menu::printMenu() {
    cout << "┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐" << endl;
    cout << "│   ██╗     ██╗   ██╗███████╗ ██████╗      █████╗ ███╗   ██╗ █████╗ ██╗     ██╗   ██╗████████╗██╗ ██████╗███████╗   │" << endl;
    cout << "│   ██║     ██║   ██║██╔════╝██╔═══██╗    ██╔══██╗████╗  ██║██╔══██╗██║     ╚██╗ ██╔╝╚══██╔══╝██║██╔════╝██╔════╝   │" << endl;
    cout << "│   ██║     ██║   ██║███████╗██║   ██║    ███████║██╔██╗ ██║███████║██║      ╚████╔╝    ██║   ██║██║     ███████╗   │" << endl;
    cout << "│   ██║     ██║   ██║╚════██║██║   ██║    ██╔══██║██║╚██╗██║██╔══██║██║       ╚██╔╝     ██║   ██║██║     ╚════██║   │" << endl;
    cout << "│   ███████╗╚██████╔╝███████║╚██████╔╝    ██║  ██║██║ ╚████║██║  ██║███████╗   ██║      ██║   ██║╚██████╗███████║   │" << endl;
    cout << "│   ╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝     ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝   ╚═╝      ╚═╝   ╚═╝ ╚═════╝╚══════╝   │" << endl;
    cout << "│                                       Portuguese Water Management Tool                                            │" << endl;
    cout << "└──┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘" << endl;
    cout << "   │" << endl;
    for (auto e : menuIndex) {
        cout << "   │  " << e << endl;
    }
    cout << "   │" << endl;
}

int Menu::run() {
    while (true) {
        clearScreen();
        printMenu();

        int choice;
        if (inputParser(choice, "Enter your choice: ")) return 1;

        clearScreen();
        switch (choice) {
            case 0:     // exit
                clearScreen();
                return 0;
            case 1:     // all data containers
                printAllDataContainer();
                break;
            case 2:     // specific data container
                if (printSpecificDataContainer()) return 1;
                break;
            case 3:     // maximum flow all cities (total)
                getTotalMaxFlow();
                printEachCityMaxFlow(bsm.getBSMGraph(), parser.getDataContainer());
                break;
            case 4:     // cities with water deficit
                printCitiesWithWaterFlowDeficit(bsm.getBSMGraph(), parser.getDataContainer());
                break;
            case 5:     // remove reservoir
                clearScreen();
                removeReservoir();
                break;
            case 9:     // reset graph
                clearScreen();
                bsm.resetBSMGraph();
                cout << "Graph reset was successful!" << endl;
                break;
            default:
                continue;
        }
        waitPress();
    }
    return 0;

/*
    printHashInfo(dataContainer, Code("C_3"));
    printAllHashInfo(dataContainer);


    for (const auto& v : parser.getCodeGraph().getVertexSet()) {
        for (const auto& e : v->getAdj()) {
            cout << "Origin: " << e->getOrig()->getInfo().getCompleteCode() << endl;
            cout << "Dest: " << e->getDest()->getInfo().getCompleteCode() << endl;
            cout << "Capacity: " << e->getWeight() << endl;
            cout << "Flow: " << e->getFlow() << endl;
            cout << "--------------------------------\n" << endl;
        }
    }



    */
    //printEachCityMaxFlow(bsm.getBSMGraph(), dataContainer);
    //printSpecificCityMaxFlow(bsm.getBSMGraph(), dataContainer, Code("C_1"));
    //printCitiesWithWaterFlowDeficit(bsm.getBSMGraph(), dataContainer);

    // check if original graph is modified by BSM functions
    /*std::stringstream ss;
    for(auto v : parser.getCodeGraph().getVertexSet()) {
        ss.str("");
        bool hasContent = false;
        for (const auto e : v->getAdj())
            if (e->getFlow() != 0) {
                ss << e->getDest()->getInfo().getCompleteCode() << "[Flow: " << e->getFlow() << "]";
                hasContent = true;
            } else continue;
        ss << ")";
        if (hasContent) std::cout << v->getInfo().getCompleteCode() << "->(" << ss.str() << std::endl;
    }*/

    return 0;
}

void Menu::printAllDataContainer() {
    printAllHashInfo(parser.getDataContainer());
}

int Menu::printSpecificDataContainer() {
    vector<string> codeType{
        "[0] Reservoir",
        "[1] Station",
        "[2] City"
    };
    for (auto e : codeType) {
        cout << e << endl;
    }

    int choice;
    if (inputParser(choice, "Enter your choice: ")) return 1;

    int codeNumber;
    if (inputParser(codeNumber, "Enter the ID number (eg. C_9, enter 9): ")) return 1;

    cout << endl;
    switch (choice) {
        case 0:
            printHashInfo(parser.getDataContainer(), Code("R_" + to_string(codeNumber)));
            break;
        case 1:
            printHashInfo(parser.getDataContainer(), Code("PS_" + to_string(codeNumber)));
            break;
        case 2:
            printHashInfo(parser.getDataContainer(), Code("C_" + to_string(codeNumber)));
            break;
        default:
            return 1;
    }

    return 0;
}

void Menu::getTotalMaxFlow() {
    cout << endl;
    cout << makeBold("Total max flow: ") << bsm.getTotalMaxFlow() << endl;
}

void Menu::getCityFlow() {
    int codeNumber;
    if (inputParser(codeNumber, "Enter the City ID number (eg. C_9, enter 9): ")) {
        cout << "ERROR: Couldn't find City" << endl;
    };

    Code cityCode("C_" + to_string(codeNumber));
    cout << makeBold("Flow to City [" + cityCode.getCompleteCode() +"]: ") << bsm.getFlowToCity(cityCode) << endl;
    cout << makeBold("City demand: ") << parser.getDataContainer().getCityHashTable().find(codeNumber)->second.getDemand() << endl;
};

void Menu::removeReservoir() {
    int codeNumber;
    if (inputParser(codeNumber, "Enter the Reservoid ID number (eg. R_9, enter 9): ")) {
        cout << "ERROR: Couldn't find Reservoid" << endl;
    }

    Code reservoirCode("R_" + to_string(codeNumber));

    map<int,double> oldCitiesFlow = bsm.getCitiesFlow();
    map<int,double> newCitiesFlow;

    if (bsm.removeReservoir(reservoirCode)) {
        cerr << "Error: Couldn't run removeReservoir of BSM" << endl;
        return;
    }

    newCitiesFlow = bsm.getCitiesFlow(); //after compile the removeReservoir

    vector<pair<City,double>> affectedCities;
    for (auto cityOld : oldCitiesFlow) {
        int cityCodeNumber = cityOld.first;
        double oldFlow = cityOld.second;
        double newFlow = newCitiesFlow[cityCodeNumber];

        if (oldFlow > newFlow) {
            auto cityObj = parser.getDataContainer().getCityHashTable().find(cityCodeNumber)->second;
            affectedCities.emplace_back(cityObj,oldFlow);
        }
    }

    cout << "New Total Max Flow: " << bsm.getTotalMaxFlow() << endl;
    cout << "CITIES AFFECTED: \"< [code] name (new-flow/demand), old-flow >\"" << endl;
    if (affectedCities.empty()) {
        cout << "     --" << endl;
        return;
    }
    int i = 1;
    for (auto pair : affectedCities) {
        auto city = pair.first;
        auto oldFlow = pair.second;
        cout << "    " << i++ << ". [" << city.getCode().getCompleteCode() << "] "
        << city.getName() << " (" << bsm.getFlowToCity(city.getCode())
        << "/" << city.getDemand() << "), " << oldFlow << endl;
    }
}