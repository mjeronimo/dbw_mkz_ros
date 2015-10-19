function data_struct = csv2mat(filename)

%% Initialization

% Load file
fid = fopen(filename);
raw_data = textscan(fid, '%f %s %f', 'delimiter', ',');
t = raw_data{1};
signals = raw_data{2};
val = raw_data{3};
fclose(fid);

% Initialize variables
data_size = length(t);
num_processed = 0;
sig_idx = 1;

% Initialize data structure
data_struct.names = {};
data_struct.time = {};
data_struct.values = {};

%% Convert CSV file
while num_processed < data_size    
    % Extract all signals of a particular type
    % and insert into output struct.
    
    sig_name = signals{1};
    idx = find(ismember(signals, sig_name));
    data_struct.names{sig_idx,1} = sig_name;
    data_struct.time{sig_idx,1} = t(idx);
    data_struct.values{sig_idx,1} = val(idx);    
    
    % Remove processed signals from further consideration.
    t = t(~ismember(signals, sig_name));
    val = val(~ismember(signals, sig_name));
    signals = signals(~ismember(signals, sig_name)); 
    
    % Move the chains
    num_processed = num_processed + length(idx);
    sig_idx = sig_idx + 1;
    
    % Progress
    disp([num2str(floor(100 * num_processed / data_size)) '% Complete'])
end