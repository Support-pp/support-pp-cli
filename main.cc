#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <experimental/filesystem>



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int DownloadScriptFromGitHub(std::string path, std::string branch, std::string scriptName){

  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if(curl) {

    std::string urlGitHub = std::string("https://raw.githubusercontent.com/Support-pp/Support-pp/") + std::string(branch.c_str()) + std::string("/support-pp.js");

    curl_easy_setopt(curl, CURLOPT_URL, urlGitHub.c_str() );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

            std::ofstream spp;
            spp.open ("/"+path+"/" + scriptName);
            spp << readBuffer;
            spp.close();
            }
}


std::string GetScriptVersion(std::string branch){

  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if(curl) {

    std::string urlGitHub = std::string("https://raw.githubusercontent.com/Support-pp/Support-pp/") + std::string(branch.c_str()) + std::string("/VERSION");

    curl_easy_setopt(curl, CURLOPT_URL, urlGitHub.c_str() );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return std::string(readBuffer);
  }
}

bool checkSinusbotPath(std::string path){
bool isRightFolder = false;;
    for (auto & p : std::experimental::filesystem::directory_iterator(std::string(path))){
      if (p == std::string(path) + "/scripts"){
        isRightFolder = true;
      }
    }
    if (!isRightFolder){
        std::cout << "[ERROR]  Can't find the sinusbot folder! Check your path!\n";
        return false;
    }
    return true;
} 

std::string getScriptName(std::string path){
std::string folderName;
    for (auto & p : std::experimental::filesystem::directory_iterator(std::string(path +"/scripts"))){
      if (p == std::string(path) + "/scripts/support-pp.js"){
        folderName = "support-pp.js";
        return folderName;
      }
      if (p == std::string(path) + "/scripts/Support.js"){
        folderName = "Support.js";
        return folderName;
      }
      if (p == std::string(path) + "/scripts/support.js"){
        folderName = "support.js";
        return folderName;
      }
    }

    return "no";

} 


int main(int argc, char* argv[])
{

      std::cout << "Support++ CLI \n" << std::endl;
      std::cout << "  supportpp <install / update / remove>  --<branch(master | dev)>\n" << std::endl;
      std::cout << "  Usage: supportpp install --master" << std::endl;
      std::cout << "  This will install the latest release.\n\n" << std::endl;
      

      if(argc < 2 || argc > 3){
            std::cout << "[ERROR] Invalid number of arguments" << std::endl;
            return 0;
      }

      std::cout << "Selected branch: ";

      //branch
      std::string branch = "-";
             if (!strcmp(argv[2],"--master") ){
                  std::cout << "-> master" << std::endl;
                  branch = "master";
             }
            if (!strcmp(argv[2],"--dev") ){
                  std::cout << "-> dev" << std::endl;
                  branch = "dev";
             }
            if (branch == "-")
                  branch = "master";

// action (install/remove/update)
      std::cout << "Selected activity: ";
      std::string action = "-";
             if (!strcmp(argv[1],"install") ){
                  std::cout << "-> install" << std::endl;
                  action = "install";
             }
            if (!strcmp(argv[1],"remove") ){
                  std::cout << "-> remove" << std::endl;
                  action = "remove";
             }
                 if (!strcmp(argv[1],"update") ){
                  std::cout << "-> update" << std::endl;
                  action = "update";
             }

            if (action == "-"){
                std::cout << "[ERROR] Wrong activity! Use (install|remove|update)" << std::endl;
              return 0;
            }
                 

//curl script versions number
      std::cout << "Version: ";
      std::cout << GetScriptVersion(branch) << std::endl;

//Set Location
      std::string sinusbotPath;
      
          std::cout << "\nPlease insert your path to the sinusbot! (/opt/sinusbot): ";
          getline (std::cin, sinusbotPath);
      

      if (!checkSinusbotPath(sinusbotPath))
        return 0;
    std::cout << "[OK] Sinusbot folder checked!\n";

//INSTALL
    if (action == "install"){
      std::cout << "[...] Download the support++ script " + GetScriptVersion(branch) + "\n";
      DownloadScriptFromGitHub(sinusbotPath + "/scripts" ,branch, "support-pp.js");
      std::cout << "[!] Finish! Good Bye.\n";
    }

//UPDATE
    if (action == "update"){
      std::cout << "[...] Update to the script version " + GetScriptVersion(branch) + "\n";
      std::string scriptName = getScriptName(sinusbotPath);
      if (scriptName == "no" ){
        std::cout << "[ERROR] Can't find the support++ script. \n";
        return 0;
      }
      DownloadScriptFromGitHub(sinusbotPath + "/scripts" ,branch, scriptName);
      std::cout << "[!] Finish! You are now up to date! Good Bye.\n";
    }

//REMOVE
    if (action == "remove"){
      std::cout << "[...] Remove the script\n";
      std::string scriptName = getScriptName(sinusbotPath);
      if (scriptName == "no" ){
        std::cout << "[ERROR] Can't find the support++ script. \n";
        return 0;
      }
      std::string rmFile = sinusbotPath + "/scripts/" + scriptName;
       if( remove( rmFile.c_str() ) != 0 )
         std::cout << "[!] Can't delete the script. Please make it manuel!\n";
       

      std::cout << "[!] Finish! We removed the script! Good Bye.\n";
    }
  return 0;
}
