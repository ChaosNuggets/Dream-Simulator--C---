#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include "pcg_random.hpp"
#include <random>
#include <string>
#include <vector>

using namespace std;

//Starts a timer
long long programStart;
long long cycleStart;

unsigned long long simulations;
unsigned long long cycles;
unsigned long long currentCycle;

vector<int> bestPearls;
vector<int> bestRods;
vector<double> bestLuck;
vector<vector<int>> bestCombined; //0th index is pearls, 1st index is rods
vector<double> worstLuck;
vector<vector<int>> worstCombined; //0th index is pearls, 1st index is rods
vector<unsigned long long> durations;

double averageBestPearls;
double averageBestPearlsLuck;
double averageBestRods;
double averageBestRodsLuck;
double averageBestLuck;
double averageBestCombined[2];
double averageBestCombinedLuck[2];
double averageWorstLuck;
double averageWorstCombined[2];
double averageWorstCombinedLuck[2];
unsigned long long averageDuration;


//Probabilities of getting the index number of pearls or better
const double pearlProbabilities[] = { 1.0000000000000107, 0.9999969186159147, 0.9999568529765519, 0.9996973707960638, 0.9985813184068676, 0.9949949962132969, 0.9858111636977509, 0.9662888729078807, 0.930856710963841, 0.8748070751342498, 0.7963035873993936, 0.6977359328241151, 0.5856716469997515, 0.46934354219447777, 0.3583222549208817, 0.2603275703015076, 0.17992167522920063, 0.11832038464961071, 0.07408182828534525, 0.04419915304618056, 0.025154167390293962, 0.013670466362054632, 0.007102923767622601, 0.0035324799028400135, 0.0016835069229116067, 0.0007697258182737056, 0.000338003926653218, 0.00014270312819157372, 5.798483311401086E-05, 2.2698056539698083E-05, 8.567645453210209E-06, 3.1211842734721166E-06, 1.09832508240634E-06, 3.736345223315971E-07, 1.2297082007249335E-07, 3.918452928671811E-08, 1.2097291712585845E-08, 3.6208608753539466E-09, 1.0513841545244768E-09, 2.963465912665034E-10, 8.112941983456862E-11, 2.158422227957923E-11, 5.583455037301624E-12, 1.4050672781815061E-12, 3.441343830272259E-13, 8.207223094197087E-14, 1.906748966313372E-14, 4.317215785145101E-15, 9.530225982536544E-16, 2.0519222689130867E-16, 4.3106138400356174E-17, 8.838806540730466E-18, 1.7695958617632592E-18, 3.460384526564832E-19, 6.61123738022084E-20, 1.2344750899402199E-20, 2.2534668355324032E-21, 4.022653101681298E-22, 7.024013562192788E-23, 1.2000073345919175E-23, 2.0064049232369355E-24, 3.2839442712072727E-25, 5.262814943657635E-26, 8.260085869367325E-27, 1.269962025622542E-27, 1.913061285186249E-28, 2.8241522581174516E-29, 4.086527942722216E-30, 5.797104551205315E-31, 8.063775764401149E-32, 1.1000541611757087E-32, 1.472017900856628E-33, 1.9324492878596948E-34, 2.489254550399009E-35, 3.146767045459153E-36, 3.9044478091462444E-37, 4.755737927070595E-38, 5.6872316231855667E-39, 6.678333038025088E-40, 7.701527931573224E-41, 8.72334445656398E-42, 9.705999905417895E-43, 1.0609642603115075E-43, 1.1395012742749344E-44, 1.2026276837566857E-45, 1.2473749968503051E-46, 1.2716216506075802E-47, 1.274259599921381E-48, 1.2552772377228394E-49, 1.2157501391225857E-50, 1.1577418940255887E-51, 1.084127553348551E-52, 9.983604812778322E-54, 9.042085784083032E-55, 8.054873698088909E-56, 7.058154012073885E-57, 6.0841236978004975E-58, 5.159534420981175E-59, 4.304854713682412E-60, 3.534034720450274E-61, 2.8547967306059527E-62, 2.2693334239494668E-63, 1.7752756742693512E-64, 1.3667915356615534E-65, 1.0356937510595892E-66, 7.724593884189577E-68, 5.670963381470131E-69, 4.098223414931532E-70, 2.9154916231026903E-71, 2.0418517536674464E-72, 1.407831946885273E-73, 9.556716220774157E-75, 6.387251506029227E-76, 4.2032199700824E-77, 2.7235005043924526E-78, 1.737655428024913E-79, 1.0917004960085831E-80, 6.753967360618652E-82, 4.11472818795585E-83, 2.4686551156965527E-84, 1.4585666959019972E-85, 8.486869014017382E-87, 4.86331031605351E-88, 2.74464953322783E-89, 1.5255225153039247E-90, 8.350889905505003E-92, 4.502296953870895E-93, 2.3907095526459958E-94, 1.2502996135056642E-95, 6.440187457544098E-97, 3.267245911871367E-98, 1.632544584433213E-99, 8.034307342796039E-101, 3.894318656290302E-102, 1.8591435012148472E-103, 8.741595225081884E-105, 4.0482006590918233E-106, 1.846389702327796E-107, 8.294121272884E-109, 3.66942461797974E-110, 1.598822555730796E-111, 6.860734631562806E-113, 2.899356400047874E-114, 1.2066602162366228E-115, 4.9455179151456384E-117, 1.996053354263532E-118, 7.933329126404113E-120, 3.1049193374153888E-121, 1.1965874306735242E-122, 4.540720013894059E-124, 1.6965913555068118E-125, 6.241474921135328E-127, 2.2606795993074447E-128, 8.061504173362708E-130, 2.8300950646510304E-131, 9.780825890245764E-133, 3.3275176807027757E-134, 1.1143340548238666E-135, 3.673157288629496E-137, 1.191710272529471E-138, 3.8052731810770444E-140, 1.1958066575364501E-141, 3.698034998906078E-143, 1.1253559436953533E-144, 3.3696943090520934E-146, 9.927607205280976E-148, 2.87754968994654E-149, 8.205306248450961E-151, 2.3015948283514265E-152, 6.3502713458654E-154, 1.7232608913934351E-155, 4.5990685947446945E-157, 1.207014100036123E-158, 3.11487477035065E-160, 7.903427608264839E-162, 1.9714986405183215E-163, 4.834389683666632E-165, 1.1652191070520267E-166, 2.7602563123906837E-168, 6.425703076997462E-170, 1.4698509160466813E-171, 3.303373163090728E-173, 7.293270623829867E-175, 1.5816640532997107E-176, 3.368825029223218E-178, 7.046251876926692E-180, 1.4470869899339606E-181, 2.9176093963248206E-183, 5.7742141543862485E-185, 1.1215713235539943E-186, 2.1377749879615437E-188, 3.9978700166595526E-190, 7.334238434078271E-192, 1.3196739668598227E-193, 2.3285551947157968E-195, 4.0284221116411224E-197, 6.831709911307497E-199, 1.1354928337971796E-200, 1.8493170250349762E-202, 2.9506559823455525E-204, 4.611167835569132E-206, 7.056507747083815E-208, 1.0571898612202587E-209, 1.5502235200877196E-211, 2.224352861580506E-213, 3.1222430576685186E-215, 4.286122632502128E-217, 5.752728241950721E-219, 7.546856861604019E-221, 9.674037865385603E-223, 1.2113226316042205E-224, 1.4810763666293389E-226, 1.7677047946659068E-228, 2.0587297292040248E-230, 2.338740534905862E-232, 2.590516770905879E-234, 2.796618805755621E-236, 2.9412726491968042E-238, 3.012287055212067E-240, 3.0026940954079874E-242, 2.9118174464573356E-244, 2.7455510960678663E-246, 2.515763771008857E-248, 2.2389041107684352E-250, 1.9340319115100103E-252, 1.6206054666689632E-254, 1.3163888283605165E-256, 1.0357988854637382E-258, 7.889041404062292E-261, 5.811446529413985E-263, 4.1370225750037155E-265, 2.843442049060369E-267, 1.88511357925725E-269, 1.2042681864808692E-271, 7.405026665039802E-274, 4.377629184302412E-276, 2.4849279346885E-278, 1.3525793281248575E-280, 7.049374912396016E-283, 3.512284458762877E-285, 1.6700651060868566E-287, 7.563768359262656E-290, 3.255496145056209E-292, 1.3292736173702507E-294, 6.008601772205656E-297, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//Probabilities of getting the index number of rods or better
const double rodProbabilities[] = { 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999997, 0.9999999999999994, 0.9999999999999986, 0.9999999999999968, 0.9999999999999919, 0.9999999999999799, 0.9999999999999504, 0.9999999999998778, 0.9999999999997039, 0.9999999999992927, 0.9999999999983368, 0.999999999996147, 0.9999999999912083, 0.9999999999802395, 0.9999999999562451, 0.9999999999045458, 0.9999999997948167, 0.9999999995653833, 0.9999999990927503, 0.9999999981334454, 0.9999999962148362, 0.9999999924334994, 0.9999999850889797, 0.9999999710294708, 0.9999999445020954, 0.9999998951661356, 0.9999998047168758, 0.9999996412443605, 0.9999993499660605, 0.9999988382609389, 0.9999979519145678, 0.9999964380663405, 0.9999938884272211, 0.9999896538092056, 0.9999827177969386, 0.9999715134694304, 0.9999536625069597, 0.9999256109945057, 0.9998821311502023, 0.9998156537023001, 0.999715392633333, 0.9995662237258452, 0.9993472822648551, 0.9990302550293414, 0.9985773589786077, 0.9979390251748176, 0.9970513422289221, 0.9958333586519955, 0.9941843962709257, 0.9919815839298022, 0.9890778767528664, 0.9853008741843708, 0.9804527813352574, 0.9743118637263802, 0.9666357167152838, 0.9571666010592598, 0.9456389819997523, 0.9317892526260995, 0.9153674306544822, 0.8961504049430155, 0.8739560935579412, 0.848657682678451, 0.8201969704390245, 0.7885957658145577, 0.7539643086918545, 0.7165057938448489, 0.6765162982649375, 0.6343797156740242, 0.5905576697794744, 0.5455747749870556, 0.5, 0.45442522501294424, 0.40944233022052556, 0.3656202843259756, 0.32348370173506236, 0.283494206155151, 0.24603569130814534, 0.21140423418544207, 0.17980302956097538, 0.15134231732154882, 0.12604390644205857, 0.10384959505698427, 0.08463256934551747, 0.06821074737390044, 0.054361018000247556, 0.04283339894074003, 0.03336428328471598, 0.025688136273619554, 0.019547218664742422, 0.014699125815628877, 0.010922123247133462, 0.00801841607019768, 0.005815603729073981, 0.00416664134800424, 0.002948657771077725, 0.0020609748251821294, 0.0014226410213920406, 0.0009697449706582897, 0.000652717735144664, 0.0004337762741545915, 0.00028460736666684937, 0.00018434629769967839, 0.00011786884979753249, 7.438900549396677E-05, 4.633749304005345E-05, 2.8486530569381342E-05, 1.7282203061193496E-05, 1.0346190794220059E-05, 6.1115727785941676E-06, 3.5619336592382746E-06, 2.0480854321207132E-06, 1.1617390608031768E-06, 6.50033939217795E-07, 3.5875563923842334E-07, 1.9528312394387842E-07, 1.0483386421237874E-07, 5.549790435883357E-08, 2.8970528960193664E-08, 1.4911019998914544E-08, 7.566500392276188E-09, 3.78516356509604E-09, 1.8665542389898576E-09, 9.072495759367648E-10, 4.3461654682280214E-10, 2.0518303754417963E-10, 9.545396788918646E-11, 4.3754694686353164E-11, 1.9760295161593174E-11, 8.791426807417161E-12, 3.852836790134139E-12, 1.6630846126595871E-12, 7.069956337622479E-13, 2.9596672694656866E-13, 1.219963524338858E-13, 4.950869638693473E-14, 1.9778736072747442E-14, 7.777651175277367E-15, 3.0100969009399206E-15, 1.1464165936989191E-15, 4.2961647552930443E-16, 1.5839480919485545E-16, 5.744683472956716E-17, 2.0492665505667078E-17, 7.189164585063035E-18, 2.4799607193624842E-18, 8.410747925327761E-19, 2.804032912489288E-19, 9.188055937619442E-20, 2.958609145303006E-20, 9.360614854599994E-21, 2.909385249928349E-21, 8.88184558335947E-22, 2.66276653230592E-22, 7.838107339025057E-23, 2.2649333607098393E-23, 6.4236372145351595E-24, 1.787723959517088E-24, 4.881164779848223E-25, 1.3072442056345083E-25, 3.4332371881338E-26, 8.840259833175907E-27, 2.2311937466153592E-27, 5.5184088855489575E-28, 1.3371629940514782E-28, 3.173469229545316E-29, 7.374713269250766E-30, 1.677621400219557E-30, 3.734678398389194E-31, 8.13374423136572E-32, 1.73248054057312E-32, 3.607811782604162E-33, 7.342913793403964E-34, 1.4601161174309068E-34, 2.835565822362937E-35, 5.375979801859604E-36, 9.946403362303494E-37, 1.7950741239234857E-37, 3.1587538182363837E-38, 5.417098899058822E-39, 9.04954195040718E-40, 1.4718943558729653E-40, 2.329633803407544E-41, 3.586072514244856E-42, 5.365597357050188E-43, 7.79863855486535E-44, 1.1003761368510237E-44, 1.506225104161551E-45, 1.9987253248533902E-46, 2.569218959517758E-47, 3.1965733547508196E-48, 3.846213246974728E-49, 4.471503535036541E-50, 5.017950463111986E-51, 5.430063485488673E-52, 5.6599379574615E-53, 5.675906288501948E-54, 5.46923367598484E-55, 5.057018170459872E-56, 4.4802430144523596E-57, 3.79714661236484E-58, 3.0733524792579524E-59, 2.371100275937144E-60, 1.740110528169608E-61, 1.2120267955473736E-62, 7.992340490760327E-64, 4.975793680580192E-65, 2.91567978891243E-66, 1.60252905565991E-67, 8.229353381359822E-69, 3.930878048324365E-70, 1.7376441574274962E-71, 7.066541259606292E-73, 2.625463827432859E-74, 8.838099199241726E-76, 2.6687340383483642E-77, 7.139288822506359E-79, 1.6656090271226152E-80, 3.3197808441722776E-82, 5.495840681049277E-84, 7.254759573069702E-86, 7.158992364111991E-88, 4.694320626190951E-90, 1.5340917079055395E-92 };

//Stores the number of rods
int rods;
//Stores the number of pearls
int pearls;


//Sets the random seed
random_device r;
unsigned int seed = r();
pcg32 pcg(r());

//Whenever uniform_dist(rng) is called, it will generate a number between 0 and 845, inclusive.
uniform_int_distribution<uint64_t> uniform_dist(0, 845);


ofstream fout;
ifstream fin;


class CommonFunctions
{
    public:
    string GetUserInput(string question)
    {
        cout << question;
        string inp;
        getline(cin, inp);
        return inp;
    }

    void Simulate262PearlsAndRods()
    {
        pearls = 0;
        rods = 0;
        //Dream did 262 barters
        for (int j = 0; j < 262; j++) {
            int random = uniform_dist(pcg);
            if (random < 40) {
                pearls++;
            }
            if (random < 423) {
                rods++;
            }
        }
    }

    void SimulateRemainingRods()
    {
        //Dream did 305 blaze kills (305-262 = 43)
        for (int j = 0; j < 43; j++) {
            if (uniform_dist(pcg) < 423) {
                rods++;
            }
        }
    }

    string TimeElapsed(long long seconds)
    {
        string returnString;
        int years = seconds/31556926;
        seconds %= 31556926;
        int months = seconds/2629744;
        seconds %= 2629744;
        int weeks = seconds/604800;
        seconds %= 604800;
        int days = seconds/86400;
        seconds %= 86400;
        int hours = seconds/3600;
        seconds %= 3600;
        int minutes = seconds/60;
        seconds %= 60;
        if (years != 0) {
            returnString += to_string(years) + "y ";
        }
        if (months != 0) {
            returnString += to_string(months) + "mo ";
        }
        if (weeks != 0) {
            returnString += to_string(weeks) + "w ";
        }
        if (days != 0) {
            returnString += to_string(days) + "d ";
        }
        if (hours != 0) {
            returnString += to_string(hours) + "h ";
        }
        if (minutes != 0) {
            returnString += to_string(minutes) + "min ";
        }
        returnString += to_string(seconds) + "s";
        return returnString;
    }
};

class Finite: public CommonFunctions
{
    private:
    void ConvertInputs(string inp, string inp2) {
        simulations = stoull(inp);
        cycles = stoull(inp2);
    }

    void ResizeVectors() {
        bestPearls.resize(cycles);
        bestRods.resize(cycles);
        bestLuck.resize(cycles);
        bestCombined.resize(cycles); //0th index is pearls, 1st index is rods
        for (int i = 0; i < cycles; i++) bestCombined[i].resize(2);
        worstLuck.resize(cycles);
        worstCombined.resize(cycles); //0th index is pearls, 1st index is rods
        for (int i = 0; i < cycles; i++) worstCombined[i].resize(2);
        durations.resize(cycles);
    }

    void FillVectors() {
        for (int i = 0; i < cycles; i++) bestLuck[i] = 1;
    }

    void StartProgramTimer() {
        programStart = time(0);
    }
    


    void StartCycleTimer() {
        cycleStart = time(0);
    }

    void Compare(int p, int r)
    {
        if (p > bestPearls[currentCycle]) {
            bestPearls[currentCycle] = p;
        }
        if (r > bestRods[currentCycle]) {
            bestRods[currentCycle] = r;
        }

        double combinedLuck = pearlProbabilities[p] * rodProbabilities[r];
        if (combinedLuck < bestLuck[currentCycle]) {
            bestLuck[currentCycle] = combinedLuck;
            bestCombined[currentCycle][0] = p;
            bestCombined[currentCycle][1] = r;
        }
        if (combinedLuck > worstLuck[currentCycle]) {
            worstLuck[currentCycle] = combinedLuck;
            worstCombined[currentCycle][0] = p;
            worstCombined[currentCycle][1] = r;
        }
    }

    //Timefreq % msgFreq should equal 0
    void SendProgressMsg(unsigned long long s, int msgFreq, int timeFreq)
    {
        if (s % msgFreq == 0) {
            cout << "Simulations completed:" << s << "/" << simulations;
            if (s % timeFreq == 0) {
                cout << " (" << TimeElapsed(time(0)-cycleStart) << ")";
            }
            cout << endl;
        }
    }

    void SimulateFinite(unsigned long long x)
    {
        for (unsigned long long i = 0; i < x; i++) {
            Simulate262PearlsAndRods();
            SimulateRemainingRods();
            Compare(pearls, rods);
            SendProgressMsg(i, 1000000, 10000000);
        }
        durations[currentCycle] = time(0)-cycleStart;
    }

    void ShowCycleResults()
    {
        cout << "\nCycle #" << currentCycle+1 << ":\n";
        cout << "Best pearl trades:\n";
        cout << bestPearls[currentCycle] << " out of 262\n";
        cout << pearlProbabilities[bestPearls[currentCycle]]*100 << "% chance of getting the same luck or better(dream got " << pearlProbabilities[bestPearls[currentCycle]] / pearlProbabilities[42] << " times luckier)\n";

        cout << "\nBest blaze drops:\n";
        cout << bestRods[currentCycle] << " out of 305\n";
        cout << rodProbabilities[bestRods[currentCycle]]*100 << "% chance of getting the same luck or better(dream got " << rodProbabilities[bestRods[currentCycle]] / rodProbabilities[211] << " times luckier)\n";

        cout << "\nBest combined run:\n";
        cout << bestCombined[currentCycle][0] << " pearl trades out of 262\n";
        cout << bestCombined[currentCycle][1] << " blaze drops out of 305\n";
        cout << pearlProbabilities[bestCombined[currentCycle][0]]*100 << "% chance of getting the same pearl luck or better(dream got " << pearlProbabilities[bestCombined[currentCycle][0]] / pearlProbabilities[42] << " times luckier)\n";
        cout << rodProbabilities[bestCombined[currentCycle][1]]*100 << "% chance of getting the same rod luck or better(dream got " << rodProbabilities[bestCombined[currentCycle][1]] / rodProbabilities[211] << " times luckier)\n";
        cout << (bestLuck[currentCycle]*100) << "% chance of getting the same luck or better on both items(dream got " << bestLuck[currentCycle] / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        cout << "\nWorst combined run:\n";
        cout << worstCombined[currentCycle][0] << " pearl trades out of 262\n";
        cout << worstCombined[currentCycle][1] << " blaze drops out of 305\n";
        cout << pearlProbabilities[worstCombined[currentCycle][0]]*100 << "% chance of getting the same pearl luck or better(dream got " << pearlProbabilities[worstCombined[currentCycle][0]] / pearlProbabilities[42] << " times luckier)\n";
        cout << rodProbabilities[worstCombined[currentCycle][1]]*100 << "% chance of getting the same rod luck or better(dream got " << rodProbabilities[worstCombined[currentCycle][1]] / rodProbabilities[211] << " times luckier)\n";
        cout << worstLuck[currentCycle]*100 << "% chance of getting the same luck or better on both items(dream got " << worstLuck[currentCycle] / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        cout << "\nDream's run:\n";
        cout << "42 pearl trades out of 262\n";
        cout << "211 blaze drops out of 305\n";
        cout << pearlProbabilities[42]*100 << "% chance of getting the same pearl luck or better\n";
        cout << rodProbabilities[211]*100 << "% chance of getting the same rod luck or better\n";
        cout << pearlProbabilities[42]*rodProbabilities[211]*100 << "% chance of getting the same luck or better on both items\n";

        cout << endl;
        cout << "Finished in " << TimeElapsed(durations[currentCycle]) << endl;
        cout << endl;
    }

    void SimulateCycles() {
        for (currentCycle = 0; currentCycle < cycles; currentCycle++) {
            StartCycleTimer();
            cout << "\nStarting cycle " << currentCycle+1 << ":\n";
            SimulateFinite(simulations);
            ShowCycleResults();
        }
    }



    void SumVectors() {
        for (int i = 0; i < cycles; i++) {
            averageBestPearls += bestPearls[i];
            averageBestPearlsLuck += pearlProbabilities[bestPearls[i]];
            averageBestRods += bestRods[i];
            averageBestRodsLuck += rodProbabilities[bestRods[i]];
            averageBestLuck += bestLuck[i];
            averageBestCombined[0] += bestCombined[i][0];
            averageBestCombined[1] += bestCombined[i][1];
            averageBestCombinedLuck[0] += pearlProbabilities[bestCombined[i][0]];
            averageBestCombinedLuck[1] += rodProbabilities[bestCombined[i][1]];
            averageWorstLuck += worstLuck[i];
            averageWorstCombined[0] += worstCombined[i][0];
            averageWorstCombined[1] += worstCombined[i][1];
            averageWorstCombinedLuck[0] += pearlProbabilities[worstCombined[i][0]];
            averageWorstCombinedLuck[1] += rodProbabilities[worstCombined[i][1]];
            averageDuration += durations[i];
        }
    }

    void DivideByCycles() {
        averageBestPearls /= cycles;
        averageBestPearlsLuck /= cycles;
        averageBestRods /= cycles;
        averageBestRodsLuck /= cycles;
        averageBestLuck /= cycles;
        averageBestCombined[0] /= cycles;
        averageBestCombined[1] /= cycles;
        averageBestCombinedLuck[0] /= cycles;
        averageBestCombinedLuck[1] /= cycles;
        averageWorstLuck /= cycles;
        averageWorstCombined[0] /= cycles;
        averageWorstCombined[1] /= cycles;
        averageWorstCombinedLuck[0] /= cycles;
        averageWorstCombinedLuck[1] /= cycles;
        averageDuration = round(averageDuration/cycles);
    }

    void CalculateAverages() {
        SumVectors();
        DivideByCycles();
    }

    void ShowAverageResults()
    {
        cout << "\nAverage results:\n";
        cout << "Best pearl trades:\n";
        cout << averageBestPearls << " out of 262\n";
        cout << averageBestPearlsLuck*100 << "% chance of getting the same luck or better(dream got " << averageBestPearlsLuck / pearlProbabilities[42] << " times luckier)\n";

        cout << "\nBest blaze drops:\n";
        cout << averageBestRods << " out of 305\n";
        cout << averageBestRodsLuck*100 << "% chance of getting the same luck or better(dream got " << averageBestRodsLuck / rodProbabilities[211] << " times luckier)\n";

        cout << "\nBest combined run:\n";
        cout << averageBestCombined[0] << " pearl trades out of 262\n";
        cout << averageBestCombined[1] << " blaze drops out of 305\n";
        cout << averageBestCombinedLuck[0]*100 << "% chance of getting the same pearl luck or better(dream got " << averageBestCombinedLuck[0] / pearlProbabilities[42] << " times luckier)\n";
        cout << averageBestCombinedLuck[1]*100 << "% chance of getting the same rod luck or better(dream got " << averageBestCombinedLuck[1] / rodProbabilities[211] << " times luckier)\n";
        cout << averageBestLuck*100 << "% chance of getting the same luck or better on both items(dream got " << averageBestLuck / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        cout << "\nWorst combined run:\n";
        cout << averageWorstCombined[0] << " pearl trades out of 262\n";
        cout << averageWorstCombined[1] << " blaze drops out of 305\n";
        cout << averageWorstCombinedLuck[0]*100 << "% chance of getting the same pearl luck or better(dream got " << averageWorstCombinedLuck[0] / pearlProbabilities[42] << " times luckier)\n";
        cout << averageWorstCombinedLuck[1]*100 << "% chance of getting the same rod luck or better(dream got " << averageWorstCombinedLuck[1] / rodProbabilities[211] << " times luckier)\n";
        cout << averageWorstLuck*100 << "% chance of getting the same luck or better on both items(dream got " << averageWorstLuck / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        cout << "\nDream's run:\n";
        cout << "42 pearl trades out of 262\n";
        cout << "211 blaze drops out of 305\n";
        cout << pearlProbabilities[42]*100 << "% chance of getting the same pearl luck or better\n";
        cout << rodProbabilities[211]*100 << "% chance of getting the same rod luck or better\n";
        cout << pearlProbabilities[42]*rodProbabilities[211]*100 << "% chance of getting the same luck or better on both items\n";

        cout << endl;
        cout << "Average duration: " << TimeElapsed(averageDuration) << endl;
        cout << endl;
    }


    public:
    void HandleInputs(string inp, string inp2)
    {
        try {
            ConvertInputs(inp, inp2);
            ResizeVectors();
            FillVectors();
            StartProgramTimer();
            SimulateCycles();
            CalculateAverages();
            ShowAverageResults();
            cout << "Done! (" << TimeElapsed(time(0)-programStart) << ")\n";
        }
        catch (...) {
            cout << "Ur blad\n";
            cout << "Type a whole number from 0 - 18,446,744,073,709,551,615.\n";
        }
    }
};

class Infinite: public CommonFunctions
{
    public:
    bool Is_empty(ifstream& pFile) {
        return pFile.peek() == ifstream::traits_type::eof();
    }

    void WriteDefaultText() {
        fout << "Simulations: 0\n";
        fout << "Simulation time: 0s\n";

        fout << "\nBest pearl trades:\n";
        fout << bestPearls[currentCycle] << " out of 262\n";
        fout << pearlProbabilities[bestPearls[currentCycle]]*100 << "% chance of getting the same luck or better(dream got " << pearlProbabilities[bestPearls[currentCycle]] / pearlProbabilities[42] << " times luckier)\n";

        fout << "\nBest blaze drops:\n";
        fout << bestRods[currentCycle] << " out of 305\n";
        fout << rodProbabilities[bestRods[currentCycle]]*100 << "% chance of getting the same luck or better(dream got " << rodProbabilities[bestRods[currentCycle]] / rodProbabilities[211] << " times luckier)\n";

        fout << "\nBest combined run:\n";
        fout << bestCombined[currentCycle][0] << " pearl trades out of 262\n";
        fout << bestCombined[currentCycle][1] << " blaze drops out of 305\n";
        fout << pearlProbabilities[bestCombined[currentCycle][0]]*100 << "% chance of getting the same pearl luck or better(dream got " << pearlProbabilities[bestCombined[currentCycle][0]] / pearlProbabilities[42] << " times luckier)\n";
        fout << rodProbabilities[bestCombined[currentCycle][1]]*100 << "% chance of getting the same rod luck or better(dream got " << rodProbabilities[bestCombined[currentCycle][1]] / rodProbabilities[211] << " times luckier)\n";
        fout << (bestLuck[currentCycle]*100) << "% chance of getting the same luck or better on both items(dream got " << bestLuck[currentCycle] / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        fout << "\nWorst combined run:\n";
        fout << worstCombined[currentCycle][0] << " pearl trades out of 262\n";
        fout << worstCombined[currentCycle][1] << " blaze drops out of 305\n";
        fout << pearlProbabilities[worstCombined[currentCycle][0]]*100 << "% chance of getting the same pearl luck or better(dream got " << pearlProbabilities[worstCombined[currentCycle][0]] / pearlProbabilities[42] << " times luckier)\n";
        fout << rodProbabilities[worstCombined[currentCycle][1]]*100 << "% chance of getting the same rod luck or better(dream got " << rodProbabilities[worstCombined[currentCycle][1]] / rodProbabilities[211] << " times luckier)\n";
        fout << worstLuck[currentCycle]*100 << "% chance of getting the same luck or better on both items(dream got " << worstLuck[currentCycle] / (pearlProbabilities[42] * rodProbabilities[211]) << " times luckier)\n";

        fout << "\nDream's run:\n";
        fout << "42 pearl trades out of 262\n";
        fout << "211 blaze drops out of 305\n";
        fout << pearlProbabilities[42]*100 << "% chance of getting the same pearl luck or better\n";
        fout << rodProbabilities[211]*100 << "% chance of getting the same rod luck or better\n";
        fout << pearlProbabilities[42]*rodProbabilities[211]*100 << "% chance of getting the same luck or better on both items\n";
    }
};


CommonFunctions cf;
Finite fn;
Infinite in;

int main()
{
    string input = cf.GetUserInput("Simulations (a number or \"infinite\"): ");
    if (input == "infinite") {
        // Create and/or open a text file in append mode
        fout.open("log.txt");
        fin.open("log.txt");

        if (in.Is_empty(fin)) {
            cout << "empty\n";
        }
        else {
            cout << "not empty\n";
        }

        // Close the file
        fout.close();
        fin.close();
    }
    else {
        string input2 = cf.GetUserInput("Cycles: ");
        fn.HandleInputs(input, input2);
    }
    cout << "Press enter to exit";
    cin.ignore();
    return 0;
}