close all; clear all;

t = -29:1:29;

%% between
mu_between = 0;
sigma_between = 2;
traffic_between = pdf('Normal', t, mu_between, sigma_between);
% traffic_between = traffic_between / traffic_between(7);

figure;
plot(t, traffic_between);

%% begin
mu_begin = 0;
sigma_begin = 10;

traffic_begin = pdf('Normal', t, mu_begin, sigma_begin);
traffic_begin = traffic_begin / traffic_begin(30) * traffic_between(30);
traffic_begin(31:end) = traffic_between(31:end);

figure;
plot(t, traffic_begin);

%% end
mu_end = 0;
sigma_end = 10;

traffic_end = pdf('Normal', t, mu_end, sigma_end);
traffic_end = traffic_end / traffic_end(30) * traffic_between(30);
traffic_end(1:29) = traffic_between(1:29);

figure;
plot(t, traffic_end);

%% overall
t_overall = -330:1:330;

mu_overall = 0;
sigma_overall = 150;
traffic_overall = pdf('Normal', t_overall, mu_overall, sigma_overall);
traffic_overall = traffic_overall - traffic_overall(1) * ones(1,length(traffic_overall));
traffic_overall = traffic_overall / traffic_overall(331) *  traffic_between(30) * 1;

figure;
plot(t_overall, traffic_overall);

%% combined
traffic_combined = zeros(1,length(t_overall));

% traffic_end = traffic_end - traffic_between(1) * ones(1,length(traffic_end));
% traffic_begin = traffic_begin - traffic_between(1) * ones(1,length(traffic_begin));
% traffic_between = traffic_between - traffic_between(1) * ones(1,length(traffic_between));

% 7:50-8:00
traffic_combined(27:85) = traffic_begin;

% 8:50-9:00
traffic_combined(87:145) = traffic_between;

% 9:50-10:00
traffic_combined(147:205) = traffic_between;

% 10:50-11:00
traffic_combined(207:265) = traffic_between;

% 11:50-12:00
traffic_combined(267:325) = traffic_between;

% 12:50-1:00
traffic_combined(327:385) = traffic_between;

% 1:50-2:00
traffic_combined(387:445) = traffic_between;

% 2:50-3:00
traffic_combined(447:505) = traffic_between;

% 3:50-4:00
traffic_combined(507:565) = traffic_between;

% 4:50-5:00
traffic_combined(567:625) = traffic_end;

traffic_combined = traffic_combined + traffic_overall;

figure;
plot(t_overall, traffic_combined);

%% save file
hr = 7; min = 0;
traffic_output = cell(length(traffic_combined),3);
for i = 1:length(traffic_combined)
    % t_str = sprintf('%02d:%02d',hr,min);
    traffic_output(i,:) = {hr, min, traffic_combined(i)};

    min = min + 1;
    if (min >= 60)
        hr = hr + 1;
        min = 0;
    end
end

writecell(traffic_output, 'traffic.csv')