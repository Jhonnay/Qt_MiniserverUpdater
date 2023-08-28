#include "stdafx.h"
#include "MyConstants.h"

//Paths
std::string MyConstants::Strings::Path_ApplicationSettings = "\\WPF_MiniserverUpdater\\ApplicationSettings.json";
std::string MyConstants::Strings::Path_Loxone_Installation = "\\Loxone";
std::string MyConstants::Strings::Path_Changelog = "\\Changelog.txt";
std::string MyConstants::Strings::Path_Folder_for_ApplicationData = "\\WPF_MiniserverUpdater";
std::string MyConstants::Strings::Path_Folder_for_Loxone_App = "/Programs/kerberos/Loxone.exe";

/* Links */
std::string MyConstants::Strings::Link_CloudDNS = "https://dns.loxonecloud.com/";
std::string MyConstants::Strings::Link_CloudDNS_simplified = "dns.loxonecloud.com/";
std::string MyConstants::Strings::Link_LPH = "https://lph.loxone.com/main.php?snr=";
std::string MyConstants::Strings::Link_CrashLog_Server = "https://gitlab.loxone.com:8443/crashstats/miniservercrashes.php?mac=";

/* Processes */
std::string MyConstants::Strings::Process_Loxone_Config = "LoxoneConfig";
std::string MyConstants::Strings::Process_Loxone_App = "Loxone";

/* ListView */
std::string MyConstants::Strings::StartUp_Listview_MS_Status = "outdated info 😐";
std::string MyConstants::Strings::StartUp_Listview_MS_Version = "TBD";
std::string MyConstants::Strings::Listview_Refresh_MS_Configuration_Error = "error";
std::string MyConstants::Strings::Listview_Refresh_MS_Configuration_Client = "Client";
std::string MyConstants::Strings::Listview_Refresh_MS_Configuration_Standalone = "Standalone";
std::string MyConstants::Strings::Listview_Refresh_MS_Configuration_ClientGateway = "Client/Gateway";
std::string MyConstants::Strings::Listview_Updated_MS_Status = "updated ✅";
std::string MyConstants::Strings::Listview_MS_Status_Updating = "Updating";
std::string MyConstants::Strings::Listview_MS_Status_Updating_Emoji = "Updating ... 🔄";
std::string MyConstants::Strings::Listview_MS_Status_Update_finished = "Update finished";
std::string MyConstants::Strings::Listview_MS_Status_retreiving_information = "retreiving data ⏲";
std::string MyConstants::Strings::Listview_MS_Status_retreiving_information_successfull = "Info up to date ✅";
std::string MyConstants::Strings::Listview_MS_Status_retreiving_information_timeout = "Error/Timeout  ⚠";
std::string MyConstants::Strings::Listview_MS_Status_Edited = "Miniserver modified ⚠";
std::string MyConstants::Strings::Listview_ProjectName_Invalid_JSON = "Invalid JSON";
std::string MyConstants::Strings::Listview_MS_Status_AutoUpdate = "Auto Update...";
std::string MyConstants::Strings::Listview_MS_Refresh_canceled = "canceled ⛔";
std::string MyConstants::Strings::Listview_MS_Update_pending = "Update pending";

/* Bottom Status Bar */
std::string MyConstants::Strings::Statusbar_ProcessStatus_Refresh_inProgress_Text = "Miniserver Information is retreived ... ";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Refresh_inProgress_MS = "retreiving data of ";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Refresh_Information_pulled_Text = "Information pulled.";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Update_Canceled = "Update canceled! ⚠";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Updating = "Updating ... 🔄";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Update_Complete = "Miniserver(s) updated";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Update_Complete_show_MS = "Updated - ";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Update_failed = "Update failed!";
std::string MyConstants::Strings::Statusbar_TextBlockConfig_No_Config_selected = "Current Config: not selected - double click to select";
std::string MyConstants::Strings::Statusbar_ProcessStatus_Refresh_Canceled = "Refresh Canceled!";


/* Create MS Dialog */
std::string MyConstants::Strings::MS_Default_partial_SN = "504F94";
std::string MyConstants::Strings::MS_Default_dummy_SN = "504F94A00000";

/* MessageBox */
std::string MyConstants::Strings::MessageBox_OpenConfig_No_Config_selected = "No Config Selected! 🤷‍";
std::string MyConstants::Strings::MessageBox_UpdateButton_No_Config_selected = "No Config Selected! No Update will be performed!";
std::string MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1 = "Unable to open and Connect Config! There are ";
std::string MyConstants::Strings::MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2 = " Config Instances running. \nPlease Save current Projects!";
std::string MyConstants::Strings::MessageBox_ButtonUpdate_OtherUdpateRunning = "The Update Process is still running! Please Cancel the Update!";
std::string MyConstants::Strings::MessageBox_ButtonUpdate_ConfigsOpen_Error_Part1 = "Unable to perform Updates! There are ";
std::string MyConstants::Strings::MessageBox_ButtonUpdate_ConfigsOpen_Error_Part2 = " Config Instances running. In order to perform Miniserver Updates no other Config has to be open! \nPlease Save current Projects!";
std::string MyConstants::Strings::MessageBox_Update_canceled = "Update(s) canceled by user!";
std::string MyConstants::Strings::MessageBox_Update_alreadyUpdatedMS = " Miniserver(s) updated!";
std::string MyConstants::Strings::MessageBox_Update_Show_all_updatedMS_Versions = "Miniserver Updated to - ";
std::string MyConstants::Strings::MessageBox_Refresh_Information_pulled = " Miniserver Information pulled!";
std::string MyConstants::Strings::MessageBox_Refresh_Context_Copy_SNR_Error = "SNR could not be copied to Clipboard! \nPlease select a Miniserver.";
std::string MyConstants::Strings::MessageBox_Refresh_Context_Copy_Username_Error = "Username could not be copied to Clipboard! \nPlease select a Miniserver.";
std::string MyConstants::Strings::MessageBox_Changelog_Cannot_be_opened = "Changelog cannot be opened! ";
std::string MyConstants::Strings::MessageBox_Applicationsettings_saved = "Settings saved. ✅";
std::string MyConstants::Strings::MessageBox_Applicationsettings_Not_saved = "Settings were not saved! ⚠";
std::string MyConstants::Strings::MessageBox_Applicationsettings_Configuration_not_found = "Configuration JSON not found! ⚠";
std::string MyConstants::Strings::MessageBox_ApplicationSettings_No_Paths_Set = "If a CheckBox is checked, a Path has to be set! ";
std::string MyConstants::Strings::MessageBox_CraeteMSDialog_Fields_Not_Filled = "Sorry my dude 🤷‍.Please fill in all fields!";
std::string MyConstants::Strings::MessageBox_CraeteMSDialog_Fields_SerialNumber_Length = "Sorry my dude 🤷‍.Please check SN length!";
std::string MyConstants::Strings::MessageBox_UpdateLevelSet_Successfully = "Updatelevel was successfully set to: ";
std::string MyConstants::Strings::MessageBox_UpdateLevelSet_Error = "Updatelevel is still: ";
std::string MyConstants::Strings::MessageBox_AutoUpdate_Not_Possible = "AutoUpdate not possible for " + Listview_Refresh_MS_Configuration_ClientGateway;
std::string MyConstants::Strings::MessageBox_AutoUpdate_seleceted_Not_Possible = MessageBox_AutoUpdate_Not_Possible + "\n\nPlease select only standalone Miniserver.";
std::string MyConstants::Strings::MessageBox_AutoUpdate_Startet = "Autoupdate started! \nPlease be patient. \nUI will not be updated on finish!";
std::string MyConstants::Strings::MessageBox_AutoUpdate_Not_Startet = "Autoupdate not started! WebService Error!";
std::string MyConstants::Strings::MessageBox_UpdateButton_AutoUpdate_Block = "Updates aborted! Please Select Miniserver that do not auto update.";
std::string MyConstants::Strings::MessageBox_ConnectConfigButton_AutoUpdate_Block = "Autoupdate! Connecting aborted!";
std::string MyConstants::Strings::MessageBox_App_already_open = "Loxone App already open!";
std::string MyConstants::Strings::MessageBox_App_not_installed = "Loxone App not installed! Could not find: %localappdata%\\Programs\\kerberos\\Loxone.exe";
std::string MyConstants::Strings::MessageBox_FTP_Local_IP_not_defined = "FTP without Local IP does not work!";
std::string MyConstants::Strings::MessageBox_FTP_Local_Ping_failed = "Ping to MS failed! Check local IP and if you are connected to the right network!";
std::string MyConstants::Strings::MessageBox_FTP_Local_authentification_failed = "Check MS Credentials!";
std::string MyConstants::Strings::MessageBox_UpdateButton_MS_updated_OR_higher_Version = "Selected Miniserver(s) are already on higher version.";
std::string MyConstants::Strings::MessageBox_ConnecConfig_Button_Canceled = "Connecting to Config canceled and Config terminated";
std::string MyConstants::Strings::MessageBox_Button_Refresh_canceled = "Refresh canceled!";
std::string MyConstants::Strings::MessageBox_ConfigExe_Not_Valid = "Please select a real Config EXE! \nPath will not be used!";
std::string MyConstants::Strings::MessageBox_Backgroundworker_Connect_Config_already_running = "There is already a Connect Config Task running! \nPlease try again later. ";
std::string MyConstants::Strings::MessageBox_Miniserver_Json_not_valid = "Please select a valid Miniserver Json Configuration!";
std::string MyConstants::Strings::MessageBox_AutoUpdate_Already_running = "AutoUpdate already running.\nPlease wait until Update is done.";
std::string MyConstants::Strings::MessageBox_AutoUpdate_selected_Already_running = MessageBox_AutoUpdate_Already_running + "\n\nPlease select only Miniserver that do not Autoupdate.";
std::string MyConstants::Strings::MessageBox_AutoUpdate_selected_ERROR_nothing_selected = "Please select Miniservers";
std::string MyConstants::Strings::MessageBox_AutoUpdate_selected_Error_which_could_not_update = "These Miniserver could not start AutoUpdate: ";
std::string MyConstants::Strings::MessageBox_CheckUpdate_no_Internet = "Check your internet connection!";
std::string MyConstants::Strings::MessageBox_CheckUpdate_no_Update_available = "You are already on the bleeding Edge. 💪 👌";
std::string MyConstants::Strings::MessageBox_SerialNumber_duplicate = "Serial Number is already in list!";
std::string MyConstants::Strings::MessageBox_AskUser_On_New_File = "Are you sure that you want to create start a new file and remove all Minsierver from the list? Please save the JSON beforehand!";
std::string MyConstants::Strings::MessageBox_ApplicationSettingsPaths_not_correct = "Please check the file Paths of your ApplicationSettings again!";
std::string MyConstants::Strings::MessageBox_ApplicationSettings_could_not_parse = "Application Settings JSON could not be parsed!";
std::string MyConstants::Strings::MessageBox_MiniserverList_could_not_parse = "Miniserver List JSON could not be parsed!";

//WebServices
std::string MyConstants::Strings::WebService_Success_Code = "200";