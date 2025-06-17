.pragma library

function lastSeenOnline(lso) {

    if (!(lso instanceof Date)) {
        throw new TypeError("The '" + lso + "' isn't a Date");
    }

    const milliseconds = lso.getTime();

    if (milliseconds === 0) {
        return "Never";
    }

    const now = new Date().getTime();

    return checkYears((now - milliseconds) / 1000);
}

function checkYears(lso) {
    const SecInYear = 31557600; // 365.25 * 24 * 60 * 60
    const years = Math.floor(lso / SecInYear);

    if (years > 0) {
        const more = lso % SecInYear > SecInYear / 2;
        let res = more ? "More than " : "About ";
        if (years === 1) {
            res += "a year";
        } else {
            res += years + " years";
        }

        return res + " ago";
    }

    return checkMonths(lso);
}

function checkMonths(lso) {
    const SecInMonth = 2630016; // 30.44 * 24 * 60 * 60
    const months = Math.floor(lso / SecInMonth);

    if (months > 10) {
        return "About a year ago";
    } else if (months > 0) {
        const more = lso % SecInMonth > SecInMonth / 2;
        let res = more ? "More than " : "About ";
        if (months === 1) {
            res += "a month";
        } else {
            res += months + " months";
        }

        return res + " ago";
    }

    return checkDays(lso);
}

function checkDays(lso) {
    const SecInDay = 86400; // 24 * 60 * 60
    const days = Math.floor(lso / SecInDay);

    if (days > 27) {
        return "About a month ago";
    } else if (days > 0) {
        const more = lso % SecInDay > SecInDay / 2;
        let res = more ? "More than " : "About ";
        if (days === 1) {
            res += "a day";
        } else {
            res += days + " days";
        }

        return res + " ago";
    }

    return checkHours(lso);
}

function checkHours(lso) {
    const SecInHour = 3600; // 60 * 60
    const hours = Math.floor(lso / SecInHour);

    if (hours > 22) {
        return "About a day ago";
    } else if (hours > 0) {
        const more = lso % SecInHour > SecInHour / 2;
        let res = more ? "More than " : "About ";
        if (hours === 1) {
            res += "an hour";
        } else {
            res += hours + " hours";
        }

        return res + " ago";
    }

    return checkMinutes(lso);
}

function checkMinutes(lso) {
    const SecInMinute = 60;
    const minutes = Math.floor(lso / SecInMinute);

    if (minutes > 50) {
        return "About an hour ago";
    } else if (minutes > 0) {
        const more = lso % SecInMinute > SecInMinute / 2;
        let res = more ? "More than " : "About ";
        if (minutes === 1) {
            res += "a minute";
        } else {
            res += minutes + " minutes";
        }

        return res + " ago";
    }

    return "Just now";
}
