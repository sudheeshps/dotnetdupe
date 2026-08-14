#include "pch.h"
#include <string>
#include <vector>
#include <utility>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {
            namespace Internal {

                std::vector<std::string> GetPathSegments(const std::string& path) {
                    std::vector<std::string> segments;
                    std::string segment;

                    for (char c : path) {
                        if (c == '/') {
                            if (!segment.empty()) {
                                segments.push_back(segment);
                                segment.clear();
                            }
                        } else {
                            segment += c;
                        }
                    }

                    if (!segment.empty()) {
                        segments.push_back(segment);
                    }

                    return segments;
                }

                bool MatchCatchAllRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    size_t fixedCount = patternSegs.size() - 1;
                    if (pathSegs.size() < fixedCount) {
                        return false;
                    }

                    for (size_t i = 0; i < fixedCount; ++i) {
                        const std::string& patternSeg = patternSegs[i];
                        const std::string& pathSeg = pathSegs[i];
                        if (patternSeg.length() >= 2 && patternSeg.front() == '{' && patternSeg.back() == '}') {
                            std::string paramName = patternSeg.substr(1, patternSeg.length() - 2);
                            extractedParams.push_back({paramName, pathSeg});
                        } else if (patternSeg != pathSeg) {
                            return false;
                        }
                    }

                    std::string catchAllName = patternSegs.back().substr(2, patternSegs.back().length() - 3);
                    std::string restPath;
                    for (size_t i = fixedCount; i < pathSegs.size(); ++i) {
                        if (!restPath.empty()) {
                            restPath += "/";
                        }
                        restPath += pathSegs[i];
                    }

                    extractedParams.push_back({catchAllName, restPath});
                    return true;
                }

                bool MatchStandardRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    if (patternSegs.size() != pathSegs.size()) {
                        return false;
                    }

                    for (size_t i = 0; i < patternSegs.size(); ++i) {
                        const std::string& patternSeg = patternSegs[i];
                        const std::string& pathSeg = pathSegs[i];
                        if (patternSeg.length() >= 2 && patternSeg.front() == '{' && patternSeg.back() == '}') {
                            std::string paramName = patternSeg.substr(1, patternSeg.length() - 2);
                            extractedParams.push_back({paramName, pathSeg});
                        } else if (patternSeg != pathSeg) {
                            return false;
                        }
                    }

                    return true;
                }

                bool MatchRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    if (!patternSegs.empty() && patternSegs.back().length() >= 3 && patternSegs.back().front() == '{' && patternSegs.back()[1] == '*' && patternSegs.back().back() == '}') {
                        return MatchCatchAllRoute(patternSegs, pathSegs, extractedParams);
                    }

                    return MatchStandardRoute(patternSegs, pathSegs, extractedParams);
                }

                void ParseServerUrl(const std::string& sUrl, std::string& host, int& port) {
                    host = "127.0.0.1";
                    port = 5000;

                    size_t protocolPos = sUrl.find("://");
                    std::string hostPort = (protocolPos != std::string::npos) ? sUrl.substr(protocolPos + 3) : sUrl;

                    size_t colonPos = hostPort.find(':');
                    if (colonPos != std::string::npos) {
                        host = hostPort.substr(0, colonPos);
                        std::string sPort = hostPort.substr(colonPos + 1);
                        size_t slashPos = sPort.find('/');
                        if (slashPos != std::string::npos) {
                            sPort = sPort.substr(0, slashPos);
                        }
                        try {
                            port = std::stoi(sPort);
                        } catch (const std::exception&) {
                            port = 5000;
                        }
                    } else {
                        size_t slashPos = hostPort.find('/');
                        host = (slashPos != std::string::npos) ? hostPort.substr(0, slashPos) : hostPort;
                    }

                    if (host == "localhost") {
                        host = "127.0.0.1";
                    }
                }

            }
        }
    }
}
