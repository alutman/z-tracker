#library "tracker"
#include "zcommon.acs"

#define SCREENBLOCKS_CVAR "screenblocks"
#define SCREENBLOCKS_HIDE_SIZE 12

#define TRACKER_ENABLED_CVAR "tr_li_enabled"
#define SHOW_SECRET_CVAR "tr_li_show_secret"
#define SHOW_ITEM_CVAR "tr_li_show_item"
#define SHOW_MONSTER_CVAR "tr_li_show_monster"
#define COLOR_COUNTS_CVAR "tr_li_color_counts" // Color counts differently when complete
#define SHOW_TIME_CVAR "tr_li_show_time"
#define SHOW_PAR_TIME_CVAR "tr_li_show_par_time"
#define COLOR_TIME_CVAR "tr_li_color_time" // Color the time according to how close it is to par
#define SHOW_LEVEL_CVAR "tr_li_show_level"
#define X_POS_CVAR "tr_li_x_pos"
#define Y_POS_CVAR "tr_li_y_pos"

#define POWERUP_TIMER_CVAR "tr_pt_enabled"
#define POWERUP_X_POS_CVAR"tr_pt_x_pos"
#define POWERUP_Y_POS_CVAR "tr_pt_y_pos"

function int pad(int time) {
  if(time < 10) {
    return StrParam(s:"0", d:time);
  }
  else {
    return StrParam(d:time);
  }
  return 0;
}

function int timeColor(int time, int par) {
  if(par <= 0) {
    return "d"; // Impossible par, just leave green
  }
  int diff = par - time;
  int percent = (diff*100) / par;
  if(diff < 0) {
    return "j";
    // return CR_WHITE;
  }
  else if(percent < 15) {
    return "g";
    // return CR_RED;
  }
  else if(percent < 30) {
    return "f";
    // return CR_GOLD;
  }
  else {
    return "d"; //green
  }
  return 1;
}

function int timeToStr(int time) {
  int secs = time % 60;
  int mins = (time / 60) % 60;
  int hours = mins / 60;
  if (hours > 0) {
    return StrParam(s:pad(hours), s:":", s:pad(mins), s:":", s:pad(secs));
  }
  else {
    return StrParam(s:pad(mins), s:":", s:pad(secs));
  }
  return 0;
}


function void displayPowerupDuration(void) {
  int fullMessage = "";
  int radSuitTicks = GetActorPowerupTics(ActivatorTID(), "PowerIronFeet");
  if(radSuitTicks > 0) {
    fullMessage = StrParam(s:fullMessage, s:"\cd", d:radSuitTicks/35, s:"\n");  
  }
  int nvTicks = GetActorPowerupTics(ActivatorTID(), "PowerLightAmp");
  if(nvTicks > 0) {
    fullMessage = StrParam(s:fullMessage, s:"\cj", d:nvTicks/35, s:"\n");  
  }
  int godTicks = GetActorPowerupTics(ActivatorTID(), "PowerInvulnerable");
  if(godTicks > 0) {
    fullMessage = StrParam(s:fullMessage, s:"\cq", d:godTicks/35, s:"\n");  
  }
  int ghostTicks = GetActorPowerupTics(ActivatorTID(), "PowerInvisibility");
  if(ghostTicks > 0) {
    fullMessage = StrParam(s:fullMessage, s:"\cu", d:ghostTicks/35, s:"\n");  
  }
  SetFont("BIGFONT");
  int padding = 0.005;
  HudMessage(l:fullMessage; HUDMSG_PLAIN | HUDMSG_NOTWITHFULLMAP, 401, CR_WHITE, GetCVar(POWERUP_X_POS_CVAR)-padding, GetCVar(POWERUP_Y_POS_CVAR)-padding, 1873);  
}

function void displayStats(void) {
  int tagColor = "g";
  int dataColor = "d";
  int completeColor = "n";
  int countColor = 0;
  int fullMessage = "";
  if(getCVar(SHOW_SECRET_CVAR)) {
    int ts = GetLevelInfo(LEVELINFO_TOTAL_SECRETS);
    int fs = GetLevelInfo(LEVELINFO_FOUND_SECRETS);
    countColor = dataColor;
    if(GetCVar(COLOR_COUNTS_CVAR) && fs >= ts) {
      countColor = completeColor;
    }
    int secretMsg = StrParam(s:"\c", s:tagColor, s:"s: \t\t", s:"\c", s:countColor, d:fs, s:"/", d:ts);
    fullMessage = StrParam(s:fullMessage, s:secretMsg, s:"\n");
  }
  if(getCVar(SHOW_ITEM_CVAR)) {
    int ti = GetLevelInfo(LEVELINFO_TOTAL_ITEMS);
    int fi = GetLevelInfo(LEVELINFO_FOUND_ITEMS);
    countColor = dataColor;
    if(GetCVar(COLOR_COUNTS_CVAR) && fi >= ti) {
      countColor = completeColor;
    }
    fullMessage = StrParam(s:fullMessage, s:"\c", s:tagColor, s:"i: \t\t\t", s:"\c", s:countColor, d:fi, s:"/", d:ti, s:"\n");
  }
  if(getCVar(SHOW_MONSTER_CVAR)) {
    int tm = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
    int km = GetLevelInfo(LEVELINFO_KILLED_MONSTERS);
    countColor = dataColor;
    if(GetCVar(COLOR_COUNTS_CVAR) && km >= tm) {
      countColor = completeColor;
    }
    fullMessage = StrParam(s:fullMessage, s:"\c", s:tagColor, s:"k: \t\t", s:"\c", s:countColor, d:km, s:"/", d:tm, s:"\n");
  }
  if(getCVar(SHOW_TIME_CVAR)) {
    int time = Timer()/35;
    int par = GetLevelInfo(LEVELINFO_PAR_TIME);
    int timeTag = "t: ";
    int timeStr = StrParam(s:timeToStr(time));
    int parStr = StrParam(s:timeToStr(par));
    int col = dataColor;
    if(getCVar(COLOR_TIME_CVAR)) {
      col = timeColor(time, par);  
    }

    fullMessage = StrParam(s:fullMessage, s:"\c", s:tagColor, s:"t: \t\t", s:"\c", s:col, s:timeStr, s:"\n");

    if(getCVar(SHOW_PAR_TIME_CVAR)) {
      fullMessage = StrParam(s:fullMessage, s:"\t\t\t\t\t\t", s:"\cj", s:parStr, s:"\n");
    }
  }
  if(getCVar(SHOW_LEVEL_CVAR)) {

    fullMessage = StrParam(s:fullMessage, s:"\cu", n:PRINTNAME_LEVEL, s:"\n");
    fullMessage = StrParam(s:fullMessage, s:"\cc", n:PRINTNAME_LEVELNAME, s:"\n");
  }
  // Pad the bottom of the message with empty space, just a little longer than 00:00 so the time doesn't move the whole message around
  fullMessage = StrParam(s:fullMessage, s:"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\x7f");

  int padding = 0.005;
  SetFont("SMALLFONT");
  HudMessage(l:fullMessage; HUDMSG_PLAIN | HUDMSG_NOTWITHFULLMAP, 400, CR_WHITE, GetCVar(X_POS_CVAR)-padding, GetCVar(Y_POS_CVAR)-padding, 1873);  
}
bool running = false;
script 400 ENTER clientside
{
  if(!running) {
    running = true;
    while(true) {
      if(GetCVar(SCREENBLOCKS_CVAR) != SCREENBLOCKS_HIDE_SIZE) {
        if(getCVar(TRACKER_ENABLED_CVAR)) {
          displayStats();  
        }
        if(getCVar(POWERUP_TIMER_CVAR)) {
          displayPowerupDuration();
        }
        
      }
      
      Delay(1);
    }
  }
}