#pragma once


#ifndef MY_CONSTANTS_H
#define MY_CONSTANTS_H

#include <string>

class MyConstants
{
public:
    static class Strings
    {
    public:
        /* Paths */
        static std::string Path_ApplicationSettings;
        static std::string Path_Loxone_Installation;
        static std::string Path_Changelog;
        static std::string Path_Folder_for_ApplicationData;
        static std::string Path_Folder_for_Loxone_App;

        /* Links */
        static std::string Link_CloudDNS;
        static std::string Link_CloudDNS_simplified;
        static std::string Link_LPH;
        static std::string Link_CrashLog_Server;

        /* Processes */
        static std::string Process_Loxone_Config;
        static std::string Process_Loxone_App;

        /* ListView */

        static std::string StartUp_Listview_MS_Status;
        static std::string StartUp_Listview_MS_Version;
        static std::string Listview_Refresh_MS_Configuration_Error;
        static std::string Listview_Refresh_MS_Configuration_Client;
        static std::string Listview_Refresh_MS_Configuration_Standalone;
        static std::string Listview_Refresh_MS_Configuration_ClientGateway;
        static std::string Listview_Updated_MS_Status;
        static std::string Listview_MS_Status_Updating;
        static std::string Listview_MS_Status_Updating_Emoji;
        static std::string Listview_MS_Status_Update_finished;
        static std::string Listview_MS_Status_retreiving_information;
        static std::string Listview_MS_Status_retreiving_information_successfull;
        static std::string Listview_MS_Status_retreiving_information_timeout;
        static std::string Listview_MS_Status_Edited;
        static std::string Listview_ProjectName_Invalid_JSON;
        static std::string Listview_MS_Status_AutoUpdate;
        static std::string Listview_MS_Refresh_canceled;
        static std::string Listview_MS_Update_pending;

        /* Bottom Status Bar */
        static std::string Statusbar_ProcessStatus_Refresh_inProgress_Text;
        static std::string Statusbar_ProcessStatus_Refresh_inProgress_MS;
        static std::string Statusbar_ProcessStatus_Refresh_Information_pulled_Text;
        static std::string Statusbar_ProcessStatus_Update_Canceled;
        static std::string Statusbar_ProcessStatus_Updating;
        static std::string Statusbar_ProcessStatus_Update_Complete;
        static std::string Statusbar_ProcessStatus_Update_Complete_show_MS;
        static std::string Statusbar_ProcessStatus_Update_failed;
        static std::string Statusbar_TextBlockConfig_No_Config_selected;
        static std::string Statusbar_ProcessStatus_Refresh_Canceled;

        /* Create MS Dialog */
        static std::string MS_Default_partial_SN;
        static std::string MS_Default_dummy_SN;

        /* MessageBox */
        static std::string MessageBox_OpenConfig_No_Config_selected;
        static std::string MessageBox_UpdateButton_No_Config_selected;
        static std::string MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1;
        static std::string MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2;
        static std::string MessageBox_ButtonUpdate_OtherUdpateRunning;
        static std::string MessageBox_ButtonUpdate_ConfigsOpen_Error_Part1;
        static std::string MessageBox_ButtonUpdate_ConfigsOpen_Error_Part2;
        static std::string MessageBox_Update_canceled;
        static std::string MessageBox_Update_alreadyUpdatedMS;
        static std::string MessageBox_Update_Show_all_updatedMS_Versions;
        static std::string MessageBox_Refresh_Information_pulled;
        static std::string MessageBox_Refresh_Context_Copy_SNR_Error;
        static std::string MessageBox_Refresh_Context_Copy_Username_Error;
        static std::string MessageBox_Changelog_Cannot_be_opened;
        static std::string MessageBox_Applicationsettings_saved;
        static std::string MessageBox_Applicationsettings_Not_saved;
        static std::string MessageBox_Applicationsettings_Configuration_not_found;
        static std::string MessageBox_ApplicationSettings_No_Paths_Set;
        static std::string MessageBox_CraeteMSDialog_Fields_Not_Filled;
        static std::string MessageBox_CraeteMSDialog_Fields_SerialNumber_Length;
        static std::string MessageBox_UpdateLevelSet_Successfully;
        static std::string MessageBox_UpdateLevelSet_Error;
        static std::string MessageBox_AutoUpdate_Not_Possible;
        static std::string MessageBox_AutoUpdate_seleceted_Not_Possible;
        static std::string MessageBox_AutoUpdate_Startet;
        static std::string MessageBox_AutoUpdate_Not_Startet;
        static std::string MessageBox_UpdateButton_AutoUpdate_Block;
        static std::string MessageBox_ConnectConfigButton_AutoUpdate_Block;
        static std::string MessageBox_App_already_open;
        static std::string MessageBox_App_not_installed;
        static std::string MessageBox_FTP_Local_IP_not_defined;
        static std::string MessageBox_FTP_Local_Ping_failed;
        static std::string MessageBox_FTP_Local_authentification_failed;
        static std::string MessageBox_UpdateButton_MS_updated_OR_higher_Version;
        static std::string MessageBox_ConnecConfig_Button_Canceled;
        static std::string MessageBox_Button_Refresh_canceled;
        static std::string MessageBox_ConfigExe_Not_Valid;
        static std::string MessageBox_Backgroundworker_Connect_Config_already_running;
        static std::string MessageBox_Miniserver_Json_not_valid;
        static std::string MessageBox_AutoUpdate_Already_running;
        static std::string MessageBox_AutoUpdate_selected_Already_running;
        static std::string MessageBox_AutoUpdate_selected_ERROR_nothing_selected;
        static std::string MessageBox_AutoUpdate_selected_Error_which_could_not_update;
        static std::string MessageBox_CheckUpdate_no_Internet;
        static std::string MessageBox_CheckUpdate_no_Update_available;
        static std::string MessageBox_SerialNumber_duplicate;
        static std::string MessageBox_AskUser_On_New_File;
        static std::string MessageBox_ApplicationSettingsPaths_not_correct;
        static std::string MessageBox_ApplicationSettings_could_not_parse;
        static std::string MessageBox_MiniserverList_could_not_parse;

        /* WebServices */
        static std::string WebService_Success_Code;
    };
};

#endif // MY_CONSTANTS_H



