#! /bin/sh

# Run SonarQube on travis. First version was given per email by one of the SonarQube engineer.

# Install required software
installSonarQubeScanner() {
  export SONAR_SCANNER_HOME=$HOME/.sonar/sonar-scanner-2.6
  rm -rf $SONAR_SCANNER_HOME
  mkdir -p $SONAR_SCANNER_HOME
  curl -sSLo $HOME/.sonar/sonar-scanner.zip http://repo1.maven.org/maven2/org/sonarsource/scanner/cli/sonar-scanner-cli/2.6/sonar-scanner-cli-2.6.zip
  unzip $HOME/.sonar/sonar-scanner.zip -d $HOME/.sonar/
  rm $HOME/.sonar/sonar-scanner.zip
  export PATH=$SONAR_SCANNER_HOME/bin:$PATH
  export SONAR_SCANNER_OPTS="-server"
}
installBuildWrapper() {
  curl -LsS https://nemo.sonarqube.org/static/cpp/build-wrapper-linux-x86.zip > build-wrapper-linux-x86.zip
  unzip build-wrapper-linux-x86.zip
}
installSonarQubeScanner
installBuildWrapper

# triggers the compilation through the build wrapper to gather compilation database
./build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir bw-outputs make all

# and finally execute the actual SonarQube analysis (the SONAR_TOKEN is set from the travis web interface, to not expose it)
sonar-scanner -Dsonar.host.url=https://nemo.sonarqube.org -Dsonar.login=$SONAR_TOKEN