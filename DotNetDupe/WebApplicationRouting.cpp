#include "pch.h"
#include <string>
#include <vector>
#include <utility>

namespace DotNetDupe {
    namespace WebAppCore {
        namespace Builder {
            namespace Internal {

                std::vector<std::string> GetPathSegments(const std::string& path) {
                    /// Initialize: Segment token vector.
                    std::vector<std::string> segments;
                    std::string segment;

                    /// Parse: Split path by '/' delimiters.
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

                    /// Return result: Return path segments.
                    return segments;
                }

                bool MatchCatchAllRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    /// Guard: Ensure path length is at least the number of fixed pattern segments.
                    size_t fixedCount = patternSegs.size() - 1;
                    if (pathSegs.size() < fixedCount) {
                        return false;
                    }

                    /// Match: Compare prefix segments prior to the catch-all parameter.
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

                    /// Extract: Collect remaining path segments into the catch-all value.
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
                    /// Guard: Segment count must match exactly.
                    if (patternSegs.size() != pathSegs.size()) {
                        return false;
                    }

                    /// Match: Compare segments or bind route placeholder parameters.
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

                    /// Return result: Route matched successfully.
                    return true;
                }

                bool MatchRoute(const std::vector<std::string>& patternSegs, const std::vector<std::string>& pathSegs, std::vector<std::pair<std::string, std::string>>& extractedParams) {
                    /// Check: Route has wildcard catch-all pattern suffix.
                    if (!patternSegs.empty() && patternSegs.back().length() >= 3 && patternSegs.back().front() == '{' && patternSegs.back()[1] == '*' && patternSegs.back().back() == '}') {
                        return MatchCatchAllRoute(patternSegs, pathSegs, extractedParams);
                    }

                    /// Fallback: Evaluate standard exact segment matching.
                    return MatchStandardRoute(patternSegs, pathSegs, extractedParams);
                }

                void ParseServerUrl(const std::string& sUrl, std::string& host, int& port) {
                    /// Initialize: Default host and port.
                    host = "127.0.0.1";
                    port = 5000;

                    /// Parse: Strip protocol prefix if present.
                    size_t protocolPos = sUrl.find("://");
                    std::string hostPort = (protocolPos != std::string::npos) ? sUrl.substr(protocolPos + 3) : sUrl;

                    /// Parse: Extract host and optional port.
                    size_t colonPos = hostPort.find(':');
                    if (colonPos != std::string::npos) {
                        host = hostPort.substr(0, colonPos);
                        std::string sPort = hostPort.substr(colonPos + 1);
                        size_t slashPos = sPort.find('/');
                        if (slashPos != std::string::npos) sPort = sPort.substr(0, slashPos);
                        try { port = std::stoi(sPort); } catch (const std::exception&) { port = 5000; }
                    } else {
                        size_t slashPos = hostPort.find('/');
                        host = (slashPos != std::string::npos) ? hostPort.substr(0, slashPos) : hostPort;
                    }

                    /// Normalize: Translate localhost to loopback IP.
                    if (host == "localhost") host = "127.0.0.1";
                }

            }
        }
    }
}
