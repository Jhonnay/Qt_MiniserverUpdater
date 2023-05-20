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
        static const std::string Path_ApplicationSettings;
        static const std::string Path_Loxone_Installation;
        static const std::string Path_Changelog;
        static const std::string Path_Folder_for_ApplicationData;
        static const std::string Path_Folder_for_Loxone_App;

        /* Links */
        static const std::string Link_CloudDNS;
        static const std::string Link_CloudDNS_simplified;
        static const std::string Link_LPH;
        static const std::string Link_CrashLog_Server;

        /* Processes */
        static const std::string Process_Loxone_Config;
        static const std::string Process_Loxone_App;

        /* ListView */

        static const std::string StartUp_Listview_MS_Status;
        static const std::string StartUp_Listview_MS_Version;
        static const std::string Listview_Refresh_MS_Configuration_Error;
        static const std::string Listview_Refresh_MS_Configuration_Client;
        static const std::string Listview_Refresh_MS_Configuration_Standalone;
        static const std::string Listview_Refresh_MS_Configuration_ClientGateway;
        static const std::string Listview_Updated_MS_Status;
        static const std::string Listview_MS_Status_Updating;
        static const std::string Listview_MS_Status_Updating_Emoji;
        static const std::string Listview_MS_Status_Update_finished;
        static const std::string Listview_MS_Status_retreiving_information;
        static const std::string Listview_MS_Status_retreiving_information_successfull;
        static const std::string Listview_MS_Status_retreiving_information_timeout;
        static const std::string Listview_MS_Status_Edited;
        static const std::string Listview_ProjectName_Invalid_JSON;
        static const std::string Listview_MS_Status_AutoUpdate;
        static const std::string Listview_MS_Refresh_canceled;
        static const std::string Listview_MS_Update_pending;

        /* Bottom Status Bar */
        static const std::string Statusbar_ProcessStatus_Refresh_inProgress_Text;
        static const std::string Statusbar_ProcessStatus_Refresh_inProgress_MS;
        static const std::string Statusbar_ProcessStatus_Refresh_Information_pulled_Text;
        static const std::string Statusbar_ProcessStatus_Update_Canceled;
        static const std::string Statusbar_ProcessStatus_Updating;
        static const std::string Statusbar_ProcessStatus_Update_Complete;
        static const std::string Statusbar_ProcessStatus_Update_Complete_show_MS;
        static const std::string Statusbar_ProcessStatus_Update_failed;
        static const std::string Statusbar_TextBlockConfig_No_Config_selected;
        static const std::string Statusbar_ProcessStatus_Refresh_Canceled;

        /* Create MS Dialog */
        static const std::string MS_Default_partial_SN;
        static const std::string MS_Default_dummy_SN;

        /* MessageBox */
        static const std::string MessageBox_OpenConfig_No_Config_selected;
        static const std::string MessageBox_UpdateButton_No_Config_selected;
        static const std::string MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part1;
        static const std::string MessageBox_ConnectConfigButton_ConfigsOpen_Error_Part2;
        static const std::string MessageBox_ButtonUpdate_OtherUdpateRunning;
        static const std::string MessageBox_ButtonUpdate_ConfigsOpen_Error_Part1;
        static const std::string MessageBox_ButtonUpdate_ConfigsOpen_Error_Part2;
        static const std::string MessageBox_Update_canceled;
        static const std::string MessageBox_Update_alreadyUpdatedMS;
        static const std::string MessageBox_Update_Show_all_updatedMS_Versions;
        static const std::string MessageBox_Refresh_Information_pulled;
        static const std::string MessageBox_Refresh_Context_Copy_SNR_Error;
        static const std::string MessageBox_Refresh_Context_Copy_Username_Error;
        static const std::string MessageBox_Changelog_Cannot_be_opened;
        static const std::string MessageBox_Applicationsettings_saved;
        static const std::string MessageBox_Applicationsettings_Not_saved;
        static const std::string MessageBox_Applicationsettings_Configuration_not_found;
        static const std::string MessageBox_ApplicationSettings_No_Paths_Set;
        static const std::string MessageBox_CraeteMSDialog_Fields_Not_Filled;
        static const std::string MessageBox_CraeteMSDialog_Fields_SerialNumber_Length;
        static const std::string MessageBox_UpdateLevelSet_Successfully;
        static const std::string MessageBox_UpdateLevelSet_Error;
        static const std::string MessageBox_AutoUpdate_Not_Possible;
        static const std::string MessageBox_AutoUpdate_seleceted_Not_Possible;
        static const std::string MessageBox_AutoUpdate_Startet;
        static const std::string MessageBox_AutoUpdate_Not_Startet;
        static const std::string MessageBox_UpdateButton_AutoUpdate_Block;
        static const std::string MessageBox_ConnectConfigButton_AutoUpdate_Block;
        static const std::string MessageBox_App_already_open;
        static const std::string MessageBox_App_not_installed;
        static const std::string MessageBox_FTP_Local_IP_not_defined;
        static const std::string MessageBox_FTP_Local_Ping_failed;
        static const std::string MessageBox_FTP_Local_authentification_failed;
        static const std::string MessageBox_UpdateButton_MS_updated_OR_higher_Version;
        static const std::string MessageBox_ConnecConfig_Button_Canceled;
        static const std::string MessageBox_Button_Refresh_canceled;
        static const std::string MessageBox_ConfigExe_Not_Valid;
        static const std::string MessageBox_Backgroundworker_Connect_Config_already_running;
        static const std::string MessageBox_Miniserver_Json_not_valid;
        static const std::string MessageBox_AutoUpdate_Already_running;
        static const std::string MessageBox_AutoUpdate_selected_Already_running;
        static const std::string MessageBox_AutoUpdate_selected_ERROR_nothing_selected;
        static const std::string MessageBox_AutoUpdate_selected_Error_which_could_not_update;
        static const std::string MessageBox_CheckUpdate_no_Internet;
        static const std::string MessageBox_CheckUpdate_no_Update_available;
        static const std::string MessageBox_SerialNumber_duplicate;
        /* WebServices */
        static const std::string WebService_Success_Code;
    };
};

#endif // MY_CONSTANTS_H



